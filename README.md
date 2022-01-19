# Onkyo Remote Over Infrared and HTTP
This is a project to control an Onkyo receiver via a microcontroller that sends infrared commands it receives over HTTP requests.

## Parts
- 1x infrared receiver and sender. I have [used one that is no longer available](https://www.amazon.de/-/en/gp/product/B07912FFXV), which fits into casing that is included in this repository. But should work with any other module just as well.
- 1x ESP32. I used a [Wemos D1 Mini](https://www.wemos.cc/en/latest/d1/d1_mini.html), which also fits into the casing. Other ESP32 based microcontrollers should work fine, too.
- 1x casing (optional). I included a FreeCAD file which you can use to print your own case if you use the parts listed above. Feel free to modify to suit your needs in case you use different parts.

## Setup
1. Hook up the IR sender to the micro controller. I used D2 as data pin, feel free to adjust the sketch to your needs.
2. Update the SSID and password in `onkyo/onkyo.ino`.
3. Upload the sketch to your MC.

## Usage
The sketch exposes a route `/ir` you can send an IR code to via post request. Say your MC's hostname is `irremote`, you could send the IR code `0x4B36D32C` from your terminal using:

```sh
curl -d '{"code":"0x4B36D32C" }' -H "Content-Type: application/json" -X POST http://irremote/ir
```
