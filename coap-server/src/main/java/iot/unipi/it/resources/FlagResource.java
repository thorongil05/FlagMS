package iot.unipi.it.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.coap.CoAP.ResponseCode;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class FlagResource extends CoapResource {

	public FlagResource(String name) {
		super(name);
		// TODO Auto-generated constructor stub
	}
	
	public void handleGET(CoapExchange exchange) {
		exchange.respond("hello world");
	}
	
	public void handlePost(CoapExchange exchange) {
		exchange.respond(ResponseCode.CREATED);
	}

}
