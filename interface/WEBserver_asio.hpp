/*
 */

#include "../interface/WEBserver.hpp"
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#if defined(_WIN32)
  #include <boost/function.hpp>
  namespace boost {
    typedef boost::asio::detail::thread thread;
  }
#else // !defined(_WIN32)
  #include <boost/thread.hpp>
  #include <pthread.h>
  #include <signal.h>
#endif // !defined(_WIN32)

namespace w3c_sw {

    namespace webserver {


	/// Parser for incoming requests.
	class request_parser
	{
	public:
	    /// Construct ready to parse the request method.
	    request_parser();

	    /// Reset to initial parser state.
	    void reset();

	    /// Parse some data. The tribool return value is true when a complete request
	    /// has been parsed, false if the data is invalid, indeterminate when more
	    /// data is required. The InputIterator return value indicates how much of the
	    /// input has been consumed.
	    template <typename InputIterator>
	    boost::tuple<boost::tribool, InputIterator> parse(request& req,
							      InputIterator begin, InputIterator end)
	    {
		while (begin != end) {
		    boost::tribool result = consume(req, *begin++);
		    if (result || !result)
			return boost::make_tuple(result, begin);
		}
		boost::tribool result = boost::indeterminate;
		return boost::make_tuple(result, begin);
	    }

	private:
	    /// Handle the next character of input.
	    boost::tribool consume(request& req, char input);

	    /// Check if a byte is an HTTP character.
	    static bool is_char(int c);

	    /// Check if a byte is an HTTP control character.
	    static bool is_ctl(int c);

	    /// Check if a byte is defined as an HTTP tspecial character.
	    static bool is_tspecial(int c);

	    /// Check if a byte is a digit.
	    static bool is_digit(int c);

	    /// The current state of the parser.
	    enum state {
		method_start,
		method,
		uri_start,
		uri,
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	    } state_;
	};

	inline request_parser::request_parser()
	    : state_(method_start)
	{  }

	inline void request_parser::reset()
	{
	    state_ = method_start;
	}

	inline boost::tribool request_parser::consume(request& req, char input)
	{
	    switch (state_)
		{
		case method_start:
		    if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		    }
		    else {
			state_ = method;
			req.method.push_back(input);
			return boost::indeterminate;
		    }
		case method:
		    if (input == ' ') {
			state_ = uri;
			return boost::indeterminate;
		    }
		    else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		    }
		    else {
			req.method.push_back(input);
			return boost::indeterminate;
		    }
		case uri_start:
		    if (is_ctl(input)) {
			return false;
		    }
		    else {
			state_ = uri;
			req.uri.push_back(input);
			return boost::indeterminate;
		    }
		case uri:
		    if (input == ' ') {
			state_ = http_version_h;
			return boost::indeterminate;
		    }
		    else if (is_ctl(input)) {
			return false;
		    }
		    else {
			req.uri.push_back(input);
			return boost::indeterminate;
		    }
		case http_version_h:
		    if (input == 'H') {
			state_ = http_version_t_1;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_t_1:
		    if (input == 'T') {
			state_ = http_version_t_2;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_t_2:
		    if (input == 'T') {
			state_ = http_version_p;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_p:
		    if (input == 'P') {
			state_ = http_version_slash;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_slash:
		    if (input == '/') {
			req.http_version_major = 0;
			req.http_version_minor = 0;
			state_ = http_version_major_start;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_major_start:
		    if (is_digit(input)) {
			req.http_version_major = req.http_version_major * 10 + input - '0';
			state_ = http_version_major;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_major:
		    if (input == '.') {
			state_ = http_version_minor_start;
			return boost::indeterminate;
		    }
		    else if (is_digit(input)) {
			req.http_version_major = req.http_version_major * 10 + input - '0';
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_minor_start:
		    if (is_digit(input)) {
			req.http_version_minor = req.http_version_minor * 10 + input - '0';
			state_ = http_version_minor;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case http_version_minor:
		    if (input == '\r') {
			state_ = expecting_newline_1;
			return boost::indeterminate;
		    }
		    else if (is_digit(input)) {
			req.http_version_minor = req.http_version_minor * 10 + input - '0';
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case expecting_newline_1:
		    if (input == '\n') {
			state_ = header_line_start;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case header_line_start:
		    if (input == '\r') {
			state_ = expecting_newline_3;
			return boost::indeterminate;
		    }
		    else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
			state_ = header_lws;
			return boost::indeterminate;
		    }
		    else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		    }
		    else {
			req.headers.push_back(header());
			req.headers.back().name.push_back(input);
			state_ = header_name;
			return boost::indeterminate;
		    }
		case header_lws:
		    if (input == '\r') {
			state_ = expecting_newline_2;
			return boost::indeterminate;
		    }
		    else if (input == ' ' || input == '\t') {
			return boost::indeterminate;
		    }
		    else if (is_ctl(input)) {
			return false;
		    }
		    else {
			state_ = header_value;
			req.headers.back().value.push_back(input);
			return boost::indeterminate;
		    }
		case header_name:
		    if (input == ':') {
			state_ = space_before_header_value;
			return boost::indeterminate;
		    }
		    else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
			return false;
		    }
		    else {
			req.headers.back().name.push_back(input);
			return boost::indeterminate;
		    }
		case space_before_header_value:
		    if (input == ' ') {
			state_ = header_value;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case header_value:
		    if (input == '\r') {
			state_ = expecting_newline_2;
			return boost::indeterminate;
		    }
		    else if (is_ctl(input)) {
			return false;
		    }
		    else {
			req.headers.back().value.push_back(input);
			return boost::indeterminate;
		    }
		case expecting_newline_2:
		    if (input == '\n') {
			state_ = header_line_start;
			return boost::indeterminate;
		    }
		    else {
			return false;
		    }
		case expecting_newline_3:
		    return (input == '\n');
		default:
		    return false;
		}
	}

	inline bool request_parser::is_char(int c)
	{
	    return c >= 0 && c <= 127;
	}

	inline bool request_parser::is_ctl(int c)
	{
	    return (c >= 0 && c <= 31) || (c == 127);
	}

	inline bool request_parser::is_tspecial(int c)
	{
	    switch (c)
		{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
		    return true;
		default:
		    return false;
		}
	}

	inline bool request_parser::is_digit(int c)
	{
	    return c >= '0' && c <= '9';
	}

	struct asioRequest : public request {
	    virtual std::string getPath() const;
	    asioRequest();
	protected:
	    bool initialized;
	};
	inline asioRequest::asioRequest () : initialized(false) {
	    
	}
	inline std::string asioRequest::getPath () const {
	    if (!initialized)
		(const_cast<asioRequest*>(this))->url_decode();
	    return request_path;
	}
	//     void url_decode();
	//     asioRequest();
	//     virtual void crackURI();
	// protected:
	// };
	// inline asioRequest::asioRequest () {  }
	// inline void asioRequest::crackURI () {
	//     url_decode();
	// }

	/// The common handler for all incoming requests.
	/// inline void request_handler::handle_request(request& req, reply& rep) was here

	/// Represents a single connection from a client.
	class connection
	    : public boost::enable_shared_from_this<connection>,
	      private boost::noncopyable
	{
	public:
	    /// Construct a connection with the given io_service.
	    explicit connection(boost::asio::io_service& io_service,
				request_handler& handler);
	    ~connection();

	    /// Get the socket associated with the connection.
	    boost::asio::ip::tcp::socket& socket();

	    /// Start the first asynchronous operation for the connection.
	    void start();

	private:
	    /// Handle completion of a read operation.
	    void handle_read(const boost::system::error_code& e,
			     std::size_t bytes_transferred);

	    /// Handle completion of a write operation.
	    void handle_write(const boost::system::error_code& e);

	    /// Strand to ensure the connection's handlers are not called concurrently.
	    boost::asio::io_service::strand strand_;

	    /// Socket for the connection.
	    boost::asio::ip::tcp::socket socket_;

	    /// The handler used to process the incoming request.
	    request_handler& request_handler_;

	    /// Buffer for incoming data.
	    boost::array<char, 8192> buffer_;

	    /// The incoming request.
	    request* request_;

	    /// The parser for the incoming request.
	    request_parser request_parser_;

	    /// The reply to be sent back to the client.
	    reply reply_;
	};

	typedef boost::shared_ptr<connection> connection_ptr;

	inline connection::connection(boost::asio::io_service& io_service,
				      request_handler& handler)
	    : strand_(io_service),
	      socket_(io_service),
	      request_handler_(handler),
	      request_(new asioRequest()), reply_() {  }

	inline connection::~connection() {
	    delete request_;
	}

	inline boost::asio::ip::tcp::socket& connection::socket() {
	    return socket_;
	}

	inline void connection::start() {
	    socket_.async_read_some(boost::asio::buffer(buffer_),
		    strand_.wrap(
			 boost::bind(&connection::handle_read, shared_from_this(),
				     boost::asio::placeholders::error,
				     boost::asio::placeholders::bytes_transferred)));
	}

	inline void connection::handle_read(const boost::system::error_code& e,
					    std::size_t bytes_transferred) {
	    if (!e) {
		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
		  *request_, buffer_.data(), buffer_.data() + bytes_transferred);

		if (result) {
		    try {
			request_handler_.handle_request(*request_, reply_);
		    } catch (webserver::reply rep) {
			reply_ = rep;
		    }
		    std::cerr << reply_.content << std::endl;
		    boost::asio::async_write(socket_, reply_.to_buffers(),
		     strand_.wrap(
			  boost::bind(&connection::handle_write, shared_from_this(),
				      boost::asio::placeholders::error)));
		} else if (!result) {
		    reply_ = reply::stock_reply(reply::bad_request);
		    boost::asio::async_write(socket_, reply_.to_buffers(),
		     strand_.wrap(
			  boost::bind(&connection::handle_write, shared_from_this(),
				      boost::asio::placeholders::error)));
		} else {
		    socket_.async_read_some(boost::asio::buffer(buffer_),
			    strand_.wrap(
				 boost::bind(&connection::handle_read, shared_from_this(),
					     boost::asio::placeholders::error,
					     boost::asio::placeholders::bytes_transferred)));
		}
	    }

	    // If an error occurs then no new asynchronous operations are started. This
	    // means that all shared_ptr references to the connection object will
	    // disappear and the object will be destroyed automatically after this
	    // handler returns. The connection class's destructor closes the socket.
	}

	inline void connection::handle_write(const boost::system::error_code& e)
	{
	    if (!e) {
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	    }

	    // No new asynchronous operations are started. This means that all shared_ptr
	    // references to the connection object will disappear and the object will be
	    // destroyed automatically after this handler returns. The connection class's
	    // destructor closes the socket.
	}



	/// The top-level class of the HTTP server.
	class server
	    : private boost::noncopyable
	{
	public:
	    /// Construct the server to listen on the specified TCP address and port, and
	    /// serve up files from the given directory.
	    explicit server(const std::string& address, 
			    const std::string& port,
			    std::size_t thread_pool_size, 
			    request_handler& request_handler_);

	    /// Run the server's io_service loop.
	    void run();

	    /// Stop the server.
	    void stop();

	private:
	    /// Handle completion of an asynchronous accept operation.
	    void handle_accept(const boost::system::error_code& e);

	    /// The number of threads that will call io_service::run().
	    std::size_t thread_pool_size_;

	    /// The io_service used to perform asynchronous operations.
	    boost::asio::io_service io_service_;

	    /// Acceptor used to listen for incoming connections.
	    boost::asio::ip::tcp::acceptor acceptor_;

	    /// The next connection to be accepted.
	    connection_ptr new_connection_;

	    /// The handler for all incoming requests.
	    request_handler& request_handler_;
	};

	inline server::server(const std::string& address, 
			      const std::string& port,
			      std::size_t thread_pool_size, 
			      request_handler& request_handler_)
	    : thread_pool_size_(thread_pool_size),
	      acceptor_(io_service_),
	      new_connection_(new connection(io_service_, request_handler_)),
	      request_handler_(request_handler_)
	{
	    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	    boost::asio::ip::tcp::resolver resolver(io_service_);
	    boost::asio::ip::tcp::resolver::query query(address, port);
	    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	    acceptor_.open(endpoint.protocol());
	    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	    acceptor_.bind(endpoint);
	    acceptor_.listen();
	    acceptor_.async_accept(new_connection_->socket(),
				   boost::bind(&server::handle_accept, this,
					       boost::asio::placeholders::error));
	}

	inline void server::run() {
	    // Create a pool of threads to run all of the io_services.
	    std::vector<boost::shared_ptr<boost::thread> > threads;
	    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
		boost::shared_ptr<boost::thread> thread(new boost::thread(
		  boost::bind(&boost::asio::io_service::run, &io_service_)));
		threads.push_back(thread);
	    }

	    // Wait for all threads in the pool to exit.
	    for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
	}

	inline void server::stop() {
	    io_service_.stop();
	}

	inline void server::handle_accept(const boost::system::error_code& e) {
	    if (!e) {
		new_connection_->start();
		new_connection_.reset(new connection(io_service_, request_handler_));
		acceptor_.async_accept(new_connection_->socket(),
				       boost::bind(&server::handle_accept, this,
						   boost::asio::placeholders::error));
	    }
	}

    } // namespace webserver


    /* WEBserver_asio implements the WEBserver interface. This is the intended
     * interface to user code.
     * TODO:
     *   Would w3c_sw::webserver::asio be better than w3c_sw::WEBserver_asio ?
     */
    class WEBserver_asio : public WEBserver {
#if defined(_WIN32)
	static boost::function0<void> console_ctrl_function;

	static BOOL WINAPI console_ctrl_handler (DWORD ctrl_type) {
	    switch (ctrl_type)
		{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		    console_ctrl_function();
		    return TRUE;
		default:
		    return FALSE;
		}
	}

#endif // defined(_WIN32)
    protected:
	w3c_sw::webserver::server* server;
    public:
	WEBserver_asio () : server(NULL) {  }
	void stop () { server->stop(); }
	void serve (const char* address, const char* port, std::size_t num_threads, webserver::request_handler& handler) {

#if defined(_WIN32)

	    // Initialise server.
	    w3c_sw::webserver::server s(address, port, num_threads, handler);
	    server = &s;

	    // Set console control handler to allow server to be stopped.
	    console_ctrl_function = boost::bind(&w3c_sw::webserver::server::stop, &s);
	    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	    // Run the server until stopped.
	    s.run();

#else // !defined(_WIN32)

	    // Block all signals for background thread.
	    sigset_t new_mask;
	    sigfillset(&new_mask);
	    sigset_t old_mask;
	    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

	    // Run server in background thread.
	    w3c_sw::webserver::server s(address, port, num_threads, handler);
	    server = &s;
	    boost::thread t(boost::bind(&w3c_sw::webserver::server::run, &s));

	    // Restore previous signals.
	    pthread_sigmask(SIG_SETMASK, &old_mask, 0);

	    // Wait for signal indicating time to shut down.
	    sigset_t wait_mask;
	    sigemptyset(&wait_mask);
	    sigaddset(&wait_mask, SIGINT);
	    sigaddset(&wait_mask, SIGQUIT);
	    sigaddset(&wait_mask, SIGTERM);
	    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
	    int sig = 0;
	    sigwait(&wait_mask, &sig);

	    // Stop the server.
	    s.stop();
	    t.join();

#endif // !defined(_WIN32)
	    server = NULL;
	}
    };

#if defined(_WIN32)
    boost::function0<void> WEBserver_asio::console_ctrl_function = NULL;
#endif // defined(_WIN32)

} // namespace w3c_sw

#ifdef NEEDDEF_WEBSERVER
  #undef NEEDDEF_WEBSERVER
  #define WEBSERVER w3c_sw::WEBserver_asio
#endif /* NEEDDEF_WEBSERVER */

