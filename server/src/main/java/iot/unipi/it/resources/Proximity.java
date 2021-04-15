package iot.unipi.it.resources;

public class Proximity extends CoapResource {
    public Proximity(string name) {
        super(name);
        setObservable(true);
    }

    public void handleGET(CoapExchange exchange) {
        exchange.respond("hello world");
    }

    public void handlePOST(CoapExchange exchange) {
        
    }
}