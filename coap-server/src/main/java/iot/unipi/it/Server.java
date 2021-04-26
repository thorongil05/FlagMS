package iot.unipi.it;

import org.eclipse.californium.core.CaliforniumLogger;
import org.eclipse.californium.core.CoapServer;

import iot.unipi.it.resources.RegistrationResource;

public class Server extends CoapServer {

	static {
		CaliforniumLogger.disableLogging();
	}

	public void startServer() {
		this.add(new RegistrationResource("registration"));
		this.start();	
	}
	
}
