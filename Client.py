from flask import Flask, request
from flask_restx import Api, Resource, fields
import socket
import os

# Server address and port
server_address = ('127.0.0.1', 8080)

app = Flask(__name__)
api = Api(app, version='1.0', title='Course work', description='API for C++ InvertedIndex')

post_input_model = api.model(
    'Input parameters for POST InvertedIndex', {
    'User name': fields.String(required=True, description='The user name'),
    'Path': fields.String(required=True, description='The path')}
)

get_input_model = api.model(
    'Input parameters for GET InvertedIndex', {
    'name': fields.String(required=True, description='The user name')}
)


class SendData(Resource):
    @api.expect(post_input_model, validate=True)
    def post(self):
        args = api.payload
        message = f'{args["User name"]} {args["Path"]}'
        # message = "SASHA datasets\\aclImdb\\test\\neg"

        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)# Create a socket

        try:
            client_socket.connect(server_address) # Connect to the server
            print(f"Connected to {server_address}")
            client_socket.sendall(message.encode())
            data = client_socket.recv(1024)
        finally:
            client_socket.close() # Close the socket
        return {'message': f'{data}'}


class GetData(Resource):
    @api.expect(get_input_model, validate=True)
    def post(self):
        args = api.payload["name"]
        if os.path.exists(f'C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\users_data\\{args}.txt'):
            return {'message': f'File proccessed, {args}.'}
        else:
            return {'message': f'File not proccessed {args}.'}


api.add_resource(SendData, '/SendPathtoExecute')
api.add_resource(GetData, '/GetExecutedData')

if __name__ == '__main__':
    app.run(debug=True)
