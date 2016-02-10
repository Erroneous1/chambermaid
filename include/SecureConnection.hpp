#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/function.hpp>
#include <sstream>
#include <vector>
#include <utility>

namespace brewpipp{

	class SecureConnection {
	public:
		SecureConnection();
		virtual ~SecureConnection();
		
		virtual size_t send() throw (boost::system::system_error)=0;
		virtual size_t receive() throw (boost::system::system_error)=0;

		std::ostringstream out;
		std::istringstream in;
		static constexpr size_t max_length = 1024;
	private:
		bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx);

	};
	class SecureServer;
	class SecureSession : public SecureConnection {
		friend class SecureServer;
	public:
		SecureSession(boost::asio::io_service& ioService,
					  boost::asio::ssl::context& context);
		virtual ~SecureSession();
		virtual size_t send() throw (boost::system::system_error);
		virtual size_t receive() throw (boost::system::system_error);
	protected:
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;
	};
	class SecureClient : public SecureSession {
	public:
		SecureClient(boost::asio::io_service& ioService,
					 boost::asio::ssl::context& context,
			   const boost::asio::ip::tcp::resolver::iterator& endpoint_iterator)
		throw (boost::system::system_error);
		virtual ~SecureClient();
	};
	class SecureServer {
	public:
		SecureServer(boost::asio::io_service& ioService,
					 boost::asio::ssl::context& context,
					 const boost::asio::ip::tcp::endpoint& endpoint,
					 const std::string& chain,
					 const std::string& key,
					 const std::string& keyPassword,
					 const std::string& dhFile) throw (boost::system::system_error);
		virtual ~SecureServer();
		void accept(SecureSession& session) throw (boost::system::system_error);
		
		static void generateRSAKey ( const int& bits, const std::string& publicFilename, const std::string &privateFilename, const std::string &password) throw (boost::system::system_error);
		static void generateCertificate(const std::string& publicKey, const std::string& privateKey, const std::string& password,
								 const long& secsValid, const std::vector<std::pair<std::string,std::string> >& x509Entries,
						   const std::string& x509Filename) throw (boost::system::system_error);
	private:
		std::string getPassword() const;
		
		boost::asio::io_service& ioService_;
		boost::asio::ip::tcp::acceptor acceptor_;
		boost::asio::ssl::context& context_;
		std::string keyPassword_;
	};
}