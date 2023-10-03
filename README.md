# POC for {{placeholder}}

When mqtt client receives data in multiple chunks, events after the first chunk are missing topic. This makes it hard to distinguish, which handler should handle the incoming event.

The system crash in the logs is not directly related to the issue. It is only a consequence of the example trying to log an empty pointer.

## Steps to reproduce

1. set connection options in `Example Configuration` in sdkconfig/menuconfig
2. build, flash and run the project binary
3. send a message to topic `example/topic` which doesn't fit the default mqtt message buffer. In my case sending [large_payload.bin](./large_payload.bin) over a mosquitto broker (both local and cloud behave the same)  
```
# in my case running
mosquitto_pub -h mqtt.eclipseprojects.io -t example/topic -f large_payload.bin
``` 
4. observe the output in the monitor

## Expected output

```
I (45150) mqtt_client_cpp: MQTT_EVENT_DATA
I (45150) MQTT_EXAMPLE: Received message with msg_id: 0
I (45160) MQTT_EXAMPLE:  - event_id: 6
I (45160) MQTT_EXAMPLE:  - topic: example/topic
I (45160) MQTT_EXAMPLE:  - topic_len: 13
I (45170) MQTT_EXAMPLE:  - total_data_len: 1048576
I (45170) MQTT_EXAMPLE:  - data_len: 1005
I (45180) MQTT_EXAMPLE:  - current_data_offset: 0
I (45180) MQTT_EXAMPLE: Received in the messages topic
I (45190) mqtt_client_cpp: MQTT_EVENT_DATA
I (45190) MQTT_EXAMPLE: Received message with msg_id: 0
I (45200) MQTT_EXAMPLE:  - event_id: 6
I (45200) MQTT_EXAMPLE:  - topic: example/topic
I (45210) MQTT_EXAMPLE:  - topic_len: 13
I (45210) MQTT_EXAMPLE:  - total_data_len: 1048576
I (45220) MQTT_EXAMPLE:  - data_len: 1024
I (45220) MQTT_EXAMPLE:  - current_data_offset: 1005
```

## Actual output

```
I (45150) mqtt_client_cpp: MQTT_EVENT_DATA
I (45150) MQTT_EXAMPLE: Received message with msg_id: 0
I (45160) MQTT_EXAMPLE:  - event_id: 6
I (45160) MQTT_EXAMPLE:  - topic: example/topic
I (45160) MQTT_EXAMPLE:  - topic_len: 13
I (45170) MQTT_EXAMPLE:  - total_data_len: 1048576
I (45170) MQTT_EXAMPLE:  - data_len: 1005
I (45180) MQTT_EXAMPLE:  - current_data_offset: 0
I (45180) MQTT_EXAMPLE: Received in the messages topic
I (45190) mqtt_client_cpp: MQTT_EVENT_DATA
I (45190) MQTT_EXAMPLE: Received message with msg_id: 0
I (45200) MQTT_EXAMPLE:  - event_id: 6
I (45200) MQTT_EXAMPLE:  - topic:
I (45210) MQTT_EXAMPLE:  - topic_len: 0
I (45210) MQTT_EXAMPLE:  - total_data_len: 1048576
I (45220) MQTT_EXAMPLE:  - data_len: 1024
I (45220) MQTT_EXAMPLE:  - current_data_offset: 1005
Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandled.
```

## Full log

[monitor.log](./monitor.log)