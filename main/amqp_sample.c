#include <stdio.h>

#include "amqp_sample.h"

static const char * amqp_hostname = "ubuntu-lic37";
static const int amqp_port = 5672;
static const char * amqp_user = "guest";
static const char * amqp_pwd = "guest";
static const char * amqp_queue_name = "test_queue";
static const char * amqp_vhost = "/";
amqp_connection_state_t rabbitmq_conn;

static void try_consume()
{
    amqp_basic_consume(rabbitmq_conn, 1, amqp_cstring_bytes(amqp_queue_name),
                       amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

    while(1)
    {
        amqp_rpc_reply_t message_res;
        amqp_envelope_t envelope;

        message_res = amqp_consume_message(rabbitmq_conn, &envelope, NULL, 0);

        if (message_res.reply_type != AMQP_RESPONSE_NORMAL)
        {
            fprintf(stderr, "amqp consume message failed %d.\n", message_res.reply_type);
            break;
        }

        printf("Delivery %u, exchange %.*s routingkey %.*s\n",
            (unsigned) envelope.delivery_tag,
            (int) envelope.exchange.len, (char *) envelope.exchange.bytes,
            (int) envelope.routing_key.len, (char *) envelope.routing_key.bytes);

        if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG)
        {
            printf("Content-type: %.*s\n",
                (int) envelope.message.properties.content_type.len,
                (char *) envelope.message.properties.content_type.bytes);
        }
        printf("----\n");
    }
}

static void connect_rabbitmq()
{
    amqp_socket_t * rabbitmq_socket;
    amqp_rpc_reply_t rabbitmq_login_reply;
    int status;
    rabbitmq_conn = amqp_new_connection();
    rabbitmq_socket = amqp_tcp_socket_new(rabbitmq_conn);
    status = amqp_socket_open(rabbitmq_socket, amqp_hostname, amqp_port);

    if (status)
    {
        perror("failed to connect rabbit mq\n");
        return;
    }

    rabbitmq_login_reply = amqp_login(rabbitmq_conn, amqp_vhost, 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, amqp_user, amqp_pwd);
    if (rabbitmq_login_reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION)
    {
        perror("amqp login error\n");
        return;
    }

    amqp_channel_open(rabbitmq_conn, 1);

    try_consume();
}

static void destroy_rabbitmq_conn()
{
    amqp_channel_close(rabbitmq_conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(rabbitmq_conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(rabbitmq_conn);
}

void amqp_sample_run()
{
    connect_rabbitmq();
    destroy_rabbitmq_conn();
}
