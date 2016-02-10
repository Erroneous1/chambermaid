#include <iostream>
#include <memory>
#include <boost/filesystem.hpp>
#include "SecureConnection.hpp"

int main(int argc, char* argv[])
{
	try
	{
// 		if (argc != 4)
// 		{
// 			std::cerr << "Usage: brewpipp <private> <public> <cert>\n";
// 			return 1;
// 		}
// 		if(!boost::filesystem::exists(argv[2]) || !boost::filesystem::exists(argv[1])){
// 			brewpipp::SecureServer::generateRSAKey(2048, argv[2], argv[1], std::string());
// 		}
// 		std::cout << "Generating certificate" << std::endl;
// 		brewpipp::SecureServer::generateCertificate("public.pem", "private.pem", std::string(), 315360000L, 
// 			std::vector<std::pair<std::string,std::string> >({
// 				{"C","US"},{"O","brewpi"},{"CN","qonos"}
// 			}),"local.cert");
		boost::asio::io_service io_service;
		if(argc == 2){
			boost::asio::ssl::context context(io_service, boost::asio::ssl::context_base::tlsv12_server);
			std::istringstream reader(argv[1]);
			unsigned short port(0);
			reader >> port;
			brewpipp::SecureServer server(io_service,
										context,
										boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port),
										"local.cert",
										"private.pem",
										std::string(),
										"dh.pem");
			for(;;){
				brewpipp::SecureSession session(io_service, context);
				server.accept(session);
				std::string line;
				do{
					std::cerr << "Received " << session.receive() << " bytes" << std::endl;
					line = session.in.str();
					std::cout << "<< " << line << std::endl;
					session.out << line;
					std::cerr << "Sent " << session.send() << " bytes" << std::endl;
					if(line.compare("exit")==0){
						return 0;;
					}
				}while(!line.empty());
			}
		}else if(argc == 3){
			boost::asio::ssl::context context(io_service, boost::asio::ssl::context_base::tlsv12_client);
			context.load_verify_file("local.cert");
			std::string host(argv[2]);
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(host, argv[1]);
			brewpipp::SecureClient client(io_service, context, resolver.resolve(query));
			char line[brewpipp::SecureConnection::max_length];
			
			do{
				do{
					std::cout << ">> ";
					std::cin.getline(line,sizeof(line));
					client.out << line;
				}while(line[0] == '\0');
				std::cerr << "Sent " << client.send() << " bytes" << std::endl;
				std::cerr << "Received " << client.receive() << " bytes" << std::endl;
				std::cout << "<< " << client.in.str() << std::endl;
			}while(strlen(line) != 0);
		}else{
			std::cout << "usage: brewpipp <port> (<host>)" << std::endl;
		}
// 		c.connect(resolver.resolve(boost::asio::ip::tcp::resolver::query("www.linux.org","443")));
// 		c.out << "GET /\r\n";
// 		std::cout << "Wrote " << c.send() << " bytes" << std::endl;
// 		std::cout << "Received " << c.receive() << " bytes" << std::endl;
// 		std::cout << c.in.str();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	return 0;
}
