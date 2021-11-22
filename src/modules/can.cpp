#include "can.h"

#include "driver/twai.h"

Can::Can(std::string name, gpio_num_t rx_pin, gpio_num_t tx_pin, long baud_rate) : Module(can, name)
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(tx_pin, rx_pin, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config;
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    switch (baud_rate)
    {
    case 1000000:
        t_config = TWAI_TIMING_CONFIG_1MBITS();
        break;
    case 800000:
        t_config = TWAI_TIMING_CONFIG_800KBITS();
        break;
    case 500000:
        t_config = TWAI_TIMING_CONFIG_500KBITS();
        break;
    case 250000:
        t_config = TWAI_TIMING_CONFIG_250KBITS();
        break;
    case 125000:
        t_config = TWAI_TIMING_CONFIG_125KBITS();
        break;
    case 100000:
        t_config = TWAI_TIMING_CONFIG_100KBITS();
        break;
    case 50000:
        t_config = TWAI_TIMING_CONFIG_50KBITS();
        break;
    case 25000:
        t_config = TWAI_TIMING_CONFIG_25KBITS();
        break;
    default:
        throw std::runtime_error("invalid baud rate");
    }

    g_config.rx_queue_len = 20;
    g_config.tx_queue_len = 20;

    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());
}

void Can::step()
{
    twai_message_t message;
    while (twai_receive(&message, pdMS_TO_TICKS(0)) == ESP_OK)
    {
        if (this->subscribers.count(message.identifier))
        {
            this->subscribers[message.identifier]->handle_can_msg(
                message.identifier,
                message.data_length_code,
                message.data);
        }

        if (this->output)
        {
            printf("can %03x", message.identifier);
            if (!(message.flags & TWAI_MSG_FLAG_RTR))
            {
                for (int i = 0; i < message.data_length_code; ++i)
                {
                    printf(",%02x", message.data[i]);
                }
            }
            printf("\n");
        }
    }
}

void Can::send(uint32_t id, uint8_t data[8], bool rtr)
{
    twai_message_t message;
    message.identifier = id;
    message.flags = rtr ? TWAI_MSG_FLAG_RTR : TWAI_MSG_FLAG_NONE;
    message.data_length_code = 8;
    for (int i = 0; i < 8; ++i)
    {
        message.data[i] = data[i];
    }
    if (twai_transmit(&message, pdMS_TO_TICKS(0)) != ESP_OK)
    {
        throw std::runtime_error("could not send CAN message");
    }
}

void Can::send(uint32_t id,
               uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
               uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
               bool rtr)
{
    uint8_t data[8] = {d0, d1, d2, d3, d4, d5, d6, d7};
    this->send(id, data, rtr);
}

void Can::call(std::string method_name, std::vector<Expression *> arguments)
{
    if (method_name == "send")
    {
        Module::expect(arguments, 9, integer, integer, integer, integer, integer, integer, integer, integer, integer);
        this->send(arguments[0]->evaluate_integer(),
                   arguments[1]->evaluate_integer(),
                   arguments[2]->evaluate_integer(),
                   arguments[3]->evaluate_integer(),
                   arguments[4]->evaluate_integer(),
                   arguments[5]->evaluate_integer(),
                   arguments[6]->evaluate_integer(),
                   arguments[7]->evaluate_integer(),
                   arguments[8]->evaluate_integer());
    }
    else
    {
        Module::call(method_name, arguments);
    }
}

void Can::subscribe(uint32_t id, Module *module)
{
    if (this->subscribers.count(id))
    {
        throw std::runtime_error("there is already a subscriber for this CAN ID");
    }
    this->subscribers[id] = module;
}