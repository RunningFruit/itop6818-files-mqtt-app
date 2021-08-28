package indi.zqc.mqtt.producer.mqtt.controller;

import indi.zqc.mqtt.producer.mqtt.MqttGateway;
import indi.zqc.mqtt.producer.mqtt.model.DeviceCmd;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;


@Slf4j
@RestController
@RequestMapping(value = "/device")
public class SendController {

    @Resource
    private MqttGateway mqttGateway;

    @RequestMapping(value = "/buzzer", method = RequestMethod.GET)
    public String buzzer(@RequestParam String sendData) {
        return sendCmdByDevice("buzzer", sendData);
    }

    @RequestMapping(value = "/led", method = RequestMethod.GET)
    public String led(@RequestParam String sendData) {
        return sendCmdByDevice("led", sendData);
    }

    @RequestMapping(value = "/gps", method = RequestMethod.GET)
    public String gps(@RequestParam String sendData) {
        return sendCmdByDevice("gps", sendData);
    }

    @RequestMapping(value = "/adcChannel", method = RequestMethod.GET)
    public String adcChannel(@RequestParam String sendData) {
        return sendCmdByDevice("adcChannel", sendData);
    }

    private String sendCmdByDevice(String device, String sendData) {
        String deviceCmd = new DeviceCmd(device, sendData).toString();
        this.mqttGateway.sendToMqtt(deviceCmd);
        log.info("deviceCmd {}", deviceCmd);
        return deviceCmd;
    }

}
