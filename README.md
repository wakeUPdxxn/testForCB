# Early beta server.Created and deployed for demonstration of the main functionality.

## About functional:

### SQLITE db inside of server,wich called a "testDb.db" for testing as soon as possible)

### The server presents an tcp image server.Includes database,if you need to test it,and 2 config files(for db and server)

### All configuration files located in the configs folder.

### In the "config" you can set listening addresses and port of the server;

### In the "dbConf" you can manually set necessary variables for db connection.Also you can do the same by UI in the top menu,named as "Database settings".

### By the way,in the "Server info" menu you can also find a list on listeting interfaces and server port or change a download folder

## Steps to use

- edit configs/config.json file if you need your custom port or a different addresses pool to listen.By default port is 2323 and address is localhost.Also you can change download location there.
- create your own database or you the default("testDb.db").
- launch the server and open "Database settings" menu to complete connection data(skip it if you use default db)
- wait for images.

## Notice! If you want to close the server, it will be waiting for processed the last received messages

# Early beta client.Created and deployed for demonstration of the main functionality.

## About functional:

### Allows you to send unlimited count of imageы of the different size.But saftly limit is about 500mb

### You can manualy change connection data in the top menu

### Displays status and count of sended/sending messages in real time

## Steps to use

- open a client.exe application
- go to the top menu and click to the "connection settings"(if you want ofc) By default address set to localhost and 2323 port
- press to "Chose file" button and select all what you want,then click "send"

### if for a number of reasons the connection is interrupted, your pictures will not go anywhere.Just click to send again after reconnect.
