package indi.zqc.mqtt.producer;

import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.context.ConfigurableApplicationContext;

/**
 * mqtt生产端
 */
@SpringBootApplication
public class ProducerApplication {

    public static void main(String[] args) {
        ConfigurableApplicationContext context = new SpringApplicationBuilder(ProducerApplication.class).run(args);

    }
}
