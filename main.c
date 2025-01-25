#include <modbus/modbus.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 1502

static modbus_t* ctx = NULL;
static modbus_mapping_t* mapping = NULL;
static int socket = 0;

static void unload(void)
{
	if ( socket != -1 ) close(socket);
	modbus_mapping_free(mapping);
	modbus_close(ctx);
	modbus_free(ctx);
}

int main(void)
{
	ctx = modbus_new_tcp(IP, PORT);
	mapping = modbus_mapping_new(10,10,10,10);
	atexit(unload);
	
	mapping->tab_bits[0] = 1;
	mapping->tab_input_bits[1] = 1;
	mapping->tab_registers[2] = 20;
	mapping->tab_input_registers[3] = 30;

	for (;;) {
		socket = modbus_tcp_listen(ctx, 1);
		modbus_tcp_accept(ctx, &socket);
		bool disconnect = false;

		while ( !disconnect ) {
		
			uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
			int rc = modbus_receive(ctx, query);
			if ( rc > 0 ) {
				modbus_reply(ctx, query, rc, mapping);
			} else if ( rc == -1 ) {
				close(socket);
				disconnect = true;
			}
		}

	}

}
