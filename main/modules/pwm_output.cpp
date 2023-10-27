#include "pwm_output.h"
#include <driver/ledc.h>

PwmOutput::PwmOutput(const std::string name,
                     const gpio_num_t pin,
                     const ledc_timer_t ledc_timer,
                     const ledc_channel_t ledc_channel)
    : Module(pwm_output, name), pin(pin), ledc_timer(ledc_timer), ledc_channel(ledc_channel) {
    gpio_reset_pin(pin);

    this->properties["frequency"] = std::make_shared<IntegerVariable>(1000);
    this->properties["duty"] = std::make_shared<IntegerVariable>(128);

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = ledc_timer,
        .freq_hz = (uint32_t)this->properties.at("frequency")->integer_value,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_channel_config_t channel_config = {
        .gpio_num = pin,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = ledc_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = ledc_timer,
        .duty = (uint32_t)this->properties.at("duty")->integer_value,
        .hpoint = 0,
        .flags = {},
    };
    ledc_timer_config(&timer_config);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    ledc_channel_config(&channel_config);
    ledc_timer_pause(LEDC_HIGH_SPEED_MODE, ledc_timer);
}

void PwmOutput::step() {
    uint32_t frequency = this->properties.at("frequency")->integer_value;
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, this->ledc_timer, frequency);
    uint32_t duty = this->properties.at("duty")->integer_value;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, this->ledc_channel, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, this->ledc_channel);
    Module::step();
}

void PwmOutput::call(const std::string method_name, const std::vector<ConstExpression_ptr> arguments) {
    if (method_name == "on") {
        Module::expect(arguments, 0);
        ledc_timer_resume(LEDC_HIGH_SPEED_MODE, this->ledc_timer);
    } else if (method_name == "off") {
        Module::expect(arguments, 0);
        ledc_timer_pause(LEDC_HIGH_SPEED_MODE, this->ledc_timer);
    } else {
        Module::call(method_name, arguments);
    }
}
