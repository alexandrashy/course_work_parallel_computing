from flask import Flask
from flask_restx import Api, Resource, fields
import socket
# Server address and port
server_address = ('127.0.0.1', 8080)


app = Flask(__name__)
api = Api(app, version='1.0', title='Course work', description='API for C++ InvertedIndex')

input_model = api.model('Input parameters for InvertedIndex', {
    'User name': fields.String(required=True, description='The user name'),
    'Path': fields.String(required=True, description='The path')
})

class SendData(Resource):
    @api.expect(input_model, validate=True)
    def post(self):
        args = api.payload
        message = f'{args["User name"]} {args["Path"]}'
        # Message to send to the server
        #message = "SASHA datasets\\aclImdb\\test\\neg"

        # Create a socket
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        try:
            # Connect to the server
            client_socket.connect(server_address)
            print(f"Connected to {server_address}")
            client_socket.sendall(message.encode())
            data = client_socket.recv(1024)
            print(f"Server response: {data.decode()}")

        finally:
            # Close the socket
            client_socket.close()
        return {'message': f'{data}'}
    def get(self):
api.add_resource(SendData, '/SendPathtoExecute')

if __name__ == '__main__':
    app.run(debug=True)
