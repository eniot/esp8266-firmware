#ifndef app_controller_api_h
#define app_controller_api_h

#ifndef WEB_API_ENABLED
#define api_controller()
#else

#include "webserver.h"
#include "mqtt.h"
#include "network.h"
#include "io.h"

void _api_resp_true()
{
    _webserver.send(200, ContentJSON, "true");
}
void api_controller()
{
    _webserver.on("/api", HTTP_GET, [] {
        _webserver.send(200, ContentJSON, "[\"/api/network\",\"/api/mqtt\",\"/api/io\",\"/api/io/:gpio\",\"/api/access\",\"/api/setup\"]");
    });

    //  IO APIs
    _webserver.on("/api/io", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentJSON, io_status());
    });
    _webserver.addHandler(new StartUriRequestHandler("/io/", HTTP_POST, [](HTTPMethod method, String path) {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        ioindex_t pin = _fetch_iopin(path);
        if (!io_valid_pin(pin))
            _webserver.send(404, ContentJSON, "invalid pin");

        config_gpio_t data;
        _update_gpio_from_web(&data);
        config_gpio_save(pin, data);
        _api_resp_true();

        io_setup();
    }));

    // MQTT APIs
    _webserver.on("/api/mqtt", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentJSON, mqtt_status());
    });
    _webserver.on("/api/mqtt", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_mqtt_t data;
        _update_mqtt_from_web(&data);
        config_mqtt_save(data);
        _api_resp_true();
        mqtt_setup();
    });

    // Network APIs
    _webserver.on("/api/network", HTTP_GET, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        _webserver.send(200, ContentJSON, network_status());
    });
    _webserver.on("/api/network", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_network_t data;
        _update_network_from_web(&data);
        config_network_save(data);
        _api_resp_true();
        network_setup();
    });

    // Access APIs
    _webserver.on("/api/access", HTTP_POST, [] {
        if (!_check_auth())
            return _webserver.requestAuthentication();
        config_access_t data;
        _update_access_from_web(&data);
        config_access_save(data);
        _api_resp_true();
    });

    // Setup APIs
    _webserver.on("/api/setup", HTTP_POST, [] {
        if (config_setup_complete() && !_check_auth())
            return _webserver.requestAuthentication();
        config_setup_t data;
        _update_setup_from_web(&data);
        config_setup(data);
        _api_resp_true();
        ESP.restart();
    });
}

#endif
#endif