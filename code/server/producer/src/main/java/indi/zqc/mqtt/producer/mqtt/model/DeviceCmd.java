package indi.zqc.mqtt.producer.mqtt.model;


import indi.zqc.mqtt.producer.mqtt.utils.GsonUtil;
import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class DeviceCmd {

    private String type;
    private String cmd;

    @Override
    public String toString() {
        return GsonUtil.GsonString(this);
    }
}
