#include <stdio.h>
#include <string.h>

#include <glib-object.h>

#include "cassandra.h"
#include "protocol/thrift_protocol.h"
#include "protocol/thrift_binary_protocol.h"
#include "transport/thrift_framed_transport.h"
#include "transport/thrift_transport.h"
#include "transport/thrift_socket.h"


int main(int argc, char** argv) {
	ThriftSocket *tsocket;
	ThriftTransport *transport;
	ThriftProtocol *protocol;
	CassandraClient *client;
	CassandraIf *service;
	InvalidRequestException *ire = NULL;
	NotFoundException *nfe = NULL;
	UnavailableException *ue = NULL;
	TimedOutException *te = NULL;
	ColumnOrSuperColumn *result;
	GError *error = NULL;

	GByteArray column = {
		.data = (unsigned char *)"first",
		.len  = 5
	};

	ColumnPath *cp = NULL;

	GByteArray key = {
		.data = (unsigned char *)"jsmith",
		.len  = 6
	};

	g_type_init();

	tsocket = THRIFT_SOCKET(
			g_object_new(
				THRIFT_TYPE_SOCKET, "hostname",
				"localhost", "port", 9160, 0
				)
			);
	transport = THRIFT_TRANSPORT(
			g_object_new(
				THRIFT_TYPE_FRAMED_TRANSPORT, "transport", tsocket, 0
				)
			);
	protocol = THRIFT_PROTOCOL(
			g_object_new(
				THRIFT_TYPE_BINARY_PROTOCOL, "transport", transport, 0
				)
			);
	client = CASSANDRA_CLIENT(
			g_object_new(
				TYPE_CASSANDRA_CLIENT, "input_protocol",
				protocol, "output_protocol", protocol, 0
				)
			);
	service = CASSANDRA_IF(client);

	if (
			!thrift_transport_open(transport, 0) ||
			!thrift_transport_is_open(transport)
		 ) {
		printf("Could not connect to server\n");
		return 1;
	}
	printf("Connected to cassandra at localhost:9160\n");

	//step 1. select keyspace
	cassandra_client_set_keyspace(
			service, "weibo", &ire, &error
			);
	if (ire) {
		printf("Invalid request exception: %s\n", ire->why);
		return 1;
	}
	if (error) {
		printf("An error has occured\n");
		return 1;
	}
	printf("Selected keyspace weibo\n");

	//step 2. select column family
	cp = g_object_new(TYPE_COLUMN_PATH, 0);
	cp->column_family = "User";
	cp->column = &column;
	cp->__isset_column = TRUE;

	cassandra_client_get(
			service, &result, &key, cp, CONSISTENCY_LEVEL_QUORUM,
			&ire, &nfe, &ue, &te, &error
			);

	if (ire) {
		printf("Invalid request exception: %s\n", ire->why);
		return 1;
	}
	if (nfe) {
		printf("Row not found\n");
		return 1;
	}
	if (ue) {
		printf("Unavailable exception\n");
		return 1;
	}
	if (te) {
		printf("Timed out exception\n");
		return 1;
	}
	if (error) {
		printf("An error has occured\n");
		return 1;
	}

	printf(
			"The result is %s\n",
			strndup(
				(char *)result->column->value->data,
				result->column->value->len
				)
			);

	/* Don't forget to free resources if
	 * your program runs longer than this */

	return 0;
}
