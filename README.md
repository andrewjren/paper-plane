# Paper Plane

## Notes

1. Use lwip dns to lookup the ip address of the url for opensky
2. Use lwip to open a tcp/ip connection to the ip address 
3. Attach a recv callback to lwip for when a packet is received 
4. Send a http request string to port 80
5. recv callback will get incoming packets, put into a fifo to process. chances are response will be broken into many packets 
6. parse incoming data as it arrives 

## References

* https://github.com/raspberrypi/pico-sdk
* https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf
* https://github.com/waveshare/Pico_ePaper_Code
