#include <memory>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include <ctime>

#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "SecureConnection.hpp"

#define STRING1(x) STRING2(x)
#define STRING2(x) #x
#define LINESTR STRING1(__LINE__)

#define throw_system_error(ec, expr) throw boost::system::system_error(ec, std::string(__FILENAME__ "@" LINESTR ": ") + expr)
#define throw_system_error_ssl(expr) throw_system_error(boost::system::error_code(ERR_get_error(), boost::asio::error::get_ssl_category()),expr)

#define context_ops boost::asio::ssl::context::default_workarounds \
| boost::asio::ssl::context::single_dh_use

namespace brewpipp {
	
	SecureConnection::~SecureConnection() {}
	SecureConnection::SecureConnection() {}

	SecureSession::SecureSession(boost::asio::io_service& ioService,
								 boost::asio::ssl::context& context)
		: socket(ioService, context) {
			context.set_options(context_ops);
			
		}
	SecureSession::~SecureSession(){
		try{
			socket.lowest_layer().cancel();
			socket.lowest_layer().close();
		}catch(const boost::system::system_error& e){}
	}

	SecureClient::SecureClient(boost::asio::io_service& io_service,
								   boost::asio::ssl::context& context,
								   const boost::asio::ip::tcp::resolver::iterator& endpoint_iterator) throw (boost::system::system_error)
								: SecureSession(io_service, context){
			socket.set_verify_mode(boost::asio::ssl::verify_peer);
			boost::system::error_code error;
			socket.lowest_layer().connect(endpoint_iterator->endpoint(), error);
			if(error){
				throw_system_error(error,"connect");
			}
			socket.handshake(boost::asio::ssl::stream_base::client, error);
			if(error){
				throw_system_error(error,"handshake");
			}
	}
	SecureClient::~SecureClient(){
		try{
			socket.lowest_layer().cancel();
			socket.lowest_layer().close();
		}catch(const boost::system::system_error& e){}
	}

	size_t SecureSession::send() throw (boost::system::system_error) {
		std::string toWrite = out.str();
		out.str(std::string());
		boost::system::error_code error;
// 		std::cerr << "--write " << toWrite << std::endl;
		size_t written = boost::asio::write(socket, boost::asio::buffer(toWrite.data(), toWrite.size()), error);
		if(written != toWrite.size() || error){
			throw_system_error(error,"Couldn't write to socket");
		}
		return written;
	}
	
	size_t SecureSession::receive() throw (boost::system::system_error) {
		boost::system::error_code error;
		std::unique_ptr<char[]> buffer(new char[max_length]);
		size_t readTotal = 0;
		std::ostringstream inReader;
		for(;;) {
			size_t read = socket.read_some(boost::asio::buffer(buffer.get(), max_length), error);
// 			std::cerr << "--read " << read << std::endl;
			if(!error){
				readTotal += read;
				inReader.write(buffer.get(), read);
				if(read < max_length){ //If it is the end of this stream of data
					break;
				}
			}else if(error == boost::asio::error::eof ||
				(error.category() == boost::asio::error::get_ssl_category() && error.value() == ERR_PACK(ERR_LIB_SSL, 0, SSL_R_SHORT_READ))){
				break;
			}else{
				throw_system_error(error,"Couldn't read from socket");
			}
		}
		in.clear();
		in.str(inReader.str());
		return readTotal;
	}
	
	SecureServer::SecureServer(boost::asio::io_service& ioService,
							   boost::asio::ssl::context& context,
							   const boost::asio::ip::tcp::endpoint& endpoint,
							   const std::string& chain,
							   const std::string& key,
							   const std::string& keyPassword,
							   const std::string& dhFile) throw (boost::system::system_error)
							   : ioService_(ioService), acceptor_(ioService, endpoint), context_(context), keyPassword_(keyPassword) {
		context_.set_options(context_ops);
		context_.set_password_callback(boost::bind(&SecureServer::getPassword, this));
		context_.use_certificate_chain_file(chain);
		context_.use_private_key_file(key, boost::asio::ssl::context::pem);
		if(!boost::filesystem::exists(dhFile)){
			std::cerr << "Generating tmp dh file " << dhFile << std::endl;
			
			std::unique_ptr<BIO,void(*)(BIO*)> dhBIO(BIO_new_file(dhFile.c_str(),"w"),&BIO_free_all);
			if(!dhBIO){
				throw_system_error_ssl("Could not open "+dhFile+" for writing");
			}
			std::unique_ptr<DH,void(*)(DH*)> dh(DH_generate_parameters(2048, RSA_F4, nullptr, nullptr),&DH_free);
			if(!dh){
				throw_system_error_ssl("Could not DH_generate_parameters");
			}
			if(PEM_write_bio_DHparams(dhBIO.get(),dh.get()) != 1){
				throw_system_error_ssl("Could not write DHparams");
			}
			std::cerr << "done" << std::endl;
		}
		context_.use_tmp_dh_file(dhFile);
	}
	SecureServer::~SecureServer(){
		try{
			acceptor_.cancel();
			acceptor_.close();
		}catch(const boost::system::system_error& e){}
	}

	
	void SecureServer::accept(SecureSession& session) throw (boost::system::system_error) {
		acceptor_.accept(session.socket.lowest_layer());
		
		boost::system::error_code error;
		session.socket.handshake(boost::asio::ssl::stream_base::server, error);
		if(error){
			throw_system_error(error,"handshake");
		}
		
	}
	
	std::string SecureServer::getPassword() const { return keyPassword_; }
	
	
	
	void SecureServer::generateRSAKey (const int& bits, const std::string& publicFilename, const std::string &privateFilename, const std::string &password) throw (boost::system::system_error) {
		const unsigned long e = RSA_F4;
		
		//Generate RSA key
		std::unique_ptr<BIGNUM,void (*)(BIGNUM*)> bignum(BN_new(), &BN_free);
		int ret;
		if((ret = BN_set_word(bignum.get(), e)) != 1){
			throw_system_error_ssl("Could not set BIGNUM word");
		}
		
		std::unique_ptr<RSA,void (*)(RSA*)> rsa(RSA_new(), &RSA_free);
		if(RSA_generate_key_ex(rsa.get(), bits, bignum.get(), nullptr) != 1){
			throw_system_error_ssl("Could not generate RSA key");
		}
		
		//Save public key
		std::unique_ptr<BIO,void (*)(BIO*)> publicBIO(BIO_new_file(publicFilename.c_str(), "w"), &BIO_free_all);
		if(!publicBIO){
			throw_system_error_ssl("Could not open "+publicFilename+" for writing");
		}
		if(PEM_write_bio_RSAPublicKey(publicBIO.get(), rsa.get()) != 1){
			throw_system_error_ssl("Could not write RSA public key");
		}
		
		//Save private key
		std::unique_ptr<BIO,void (*)(BIO*)> privateBIO(BIO_new_file(privateFilename.c_str(), "w"), &BIO_free_all);
		if(!privateBIO){
			throw_system_error_ssl("Could not open "+privateFilename+" for writing");
		}
		if(password.empty()){
			if(PEM_write_bio_RSAPrivateKey(privateBIO.get(), rsa.get(), nullptr, nullptr, 0, nullptr, nullptr) != 1){
				throw_system_error_ssl("Could not write RSA private key");
			}
		}else{
			if(PEM_write_bio_RSAPrivateKey(privateBIO.get(), rsa.get(), EVP_des_ede3_cbc(), (unsigned char *)password.data(), password.size(), nullptr, nullptr) != 1){
				throw_system_error_ssl("Could not write RSA private key");
			}
		}
	}
	int pemPasswordCBFromString(char *buf, int size, int rwflag, void *userdata){
		std::string* password = static_cast<std::string*>(userdata);
		int ret = std::min(size,(int)password->size());
		if(ret < 1){
			return 0;
		}
		std::uninitialized_copy(password->data(),password->data()+ret,buf);
		return ret;
	}

	void SecureServer::generateCertificate(const std::string& publicKey, const std::string& privateKey, const std::string& password,
		const long& secsValid, const std::vector<std::pair<std::string,std::string> >& x509Entries,
		const std::string& x509Filename) throw (boost::system::system_error) {
			std::cerr << "generating certificate" << std::endl;
		std::unique_ptr<X509,void (*)(X509*)> x509(X509_new(), &X509_free);
		
		if(ASN1_INTEGER_set(X509_get_serialNumber(x509.get()), std::time(nullptr)) != 1){
			throw_system_error_ssl("Could not set X509 parameters");
		}
		X509_gmtime_adj(X509_get_notBefore(x509.get()), 0);
		X509_gmtime_adj(X509_get_notAfter(x509.get()), secsValid);
		std::unique_ptr<EVP_PKEY,void(*)(EVP_PKEY*)> privKey(EVP_PKEY_new(), &EVP_PKEY_free);
		std::unique_ptr<EVP_PKEY,void(*)(EVP_PKEY*)> pubKey(EVP_PKEY_new(), &EVP_PKEY_free);
		std::unique_ptr<BIO,void (*)(BIO*)> privateBIO(BIO_new_file(privateKey.c_str(), "r"), &BIO_free_all);
		if(!privateBIO){
			throw_system_error_ssl("Could not open "+privateKey+" for reading");
		}
		pem_password_cb* password_cb(nullptr);
		void *password_u(nullptr);
		if(!password.empty()){
			password_cb = &pemPasswordCBFromString;
			password_u = (void*)&password;
		}
		std::unique_ptr<RSA,void(*)(RSA*)> rsa(PEM_read_bio_RSAPrivateKey(privateBIO.get(), nullptr, password_cb, password_u),&RSA_free);
		if(!rsa){
			throw_system_error_ssl("Could not read PEM RSA private key from "+privateKey);
		}
		std::cerr << "read " << privateKey << std::endl;
		if(EVP_PKEY_set1_RSA(privKey.get(), rsa.get()) != 1){
			throw_system_error_ssl("Could not assign EVP_PKEY from RSA private key");
		}
		std::cerr << "assigned EVP_PKEY" << std::endl;
		std::unique_ptr<BIO,void (*)(BIO*)> publicBIO(BIO_new_file(publicKey.c_str(), "r"), &BIO_free_all);
		if(!publicBIO){
			throw_system_error_ssl("Could not open "+publicKey+" for reading");
		}
		RSA *ptr = rsa.get();
		if(PEM_read_bio_RSAPublicKey(publicBIO.get(),&ptr,nullptr,nullptr) == nullptr){
			throw_system_error_ssl("Could not read PEM RSA public key from "+publicKey);
		}
		std::cerr << "read " << publicKey << std::endl;
		if(EVP_PKEY_set1_RSA(pubKey.get(), rsa.get()) != 1){
			throw_system_error_ssl("Could not assign EVP_PKEY from RSA public key");
		}
		
		if(X509_set_pubkey(x509.get(), pubKey.get()) != 1){
			throw_system_error_ssl("Could nost assign X509 public key from EVP_PKEY");
		}
		X509_NAME *name = X509_get_subject_name(x509.get());
		std::cerr << "got subject name" << std::endl;
		for(const std::pair<std::string,std::string>& entry : x509Entries){
			if(X509_NAME_add_entry_by_txt(name, entry.first.c_str(), MBSTRING_ASC, (const unsigned char*)entry.second.c_str(), entry.second.length(), -1, 0) != 1){
				throw_system_error_ssl("Could not add X509 entry /"+entry.first+"/ = \""+entry.second+'"');
			}
			std::cerr << "added entry /" << entry.first << "/ = \"" << entry.second << '"' << std::endl;
		}
		if(X509_set_issuer_name(x509.get(),name) != 1){
			throw_system_error_ssl("Could not set X509 issuer name");
		}
		std::cerr << "set issuer name" << std::endl;
		std::unique_ptr<EVP_MD_CTX,void(*)(EVP_MD_CTX*)> mctx(EVP_MD_CTX_create(),&EVP_MD_CTX_destroy);
// 		EVP_PKEY_CTX *pkctx(nullptr);
		if(EVP_DigestSignInit(mctx.get(),nullptr,EVP_sha256(),nullptr,privKey.get()) != 1){
			throw_system_error_ssl("Could not init EVP Digest Sign");
		}
		std::cerr << "initialized EVP MD CTX" << std::endl;
		if(X509_sign_ctx(x509.get(),mctx.get()) <= 0){
			throw_system_error_ssl("Could not sign certificate");
		}
		std::cerr << "signed" << std::endl;
		std::unique_ptr<BIO,void(*)(BIO*)> x509BIO(BIO_new_file(x509Filename.c_str(),"w"),&BIO_free_all);
		if(PEM_write_bio_X509(x509BIO.get(),x509.get()) != 1){
			throw_system_error_ssl("Could not write X509 certificate");
		}
		std::cerr << "written to " << x509Filename << std::endl;
	}
	
// 	SecureSession::SecureSession ( SecureSession&& from ) : SecureConnection(), socket(std::move(from.socket)) {}

}