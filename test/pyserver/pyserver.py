from SimpleXMLRPCServer import SimpleXMLRPCServer

# Create server
server = SimpleXMLRPCServer(("localhost", 7777))
server.register_introspection_functions()

# Register an instance; all the methods of the instance are 
# published as XML-RPC methods
class TestServer:
    def sum( self, x, y ):
        return x+y
    def difference( self, x, y ):
        return x-y

server.register_instance( TestServer() )

# Run the server's main loop
server.serve_forever()

