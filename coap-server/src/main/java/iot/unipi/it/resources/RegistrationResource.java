package iot.unipi.it.resources;

import java.net.InetAddress;

import iot.unipi.it.model.Flag;
import iot.unipi.it.model.Resource;
import iot.unipi.it.model.TrackLimit;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.server.resources.CoapExchange;

import iot.unipi.it.Application;

public class RegistrationResource extends CoapResource {

	public RegistrationResource(String name) {
		super(name);
	}
	
	public void handleGET(CoapExchange exchange) {
		exchange.accept();
		InetAddress addr = exchange.getSourceAddress();
		CoapClient client = new CoapClient("coap://[" + addr.getHostAddress() + "]:5683/.well-known/core");
		CoapResponse response = client.get();
		String code = response.getCode().toString();
		if(!code.startsWith("2")) {
			System.err.println("Error with code: " + code);
			return;
		}
		
		String responseText = response.getResponseText();
		
		System.out.println("Response: " + responseText);
		
		String[] resources = responseText.split(",");
		
		for(int i = 1; i < resources.length; i++) {
			//1 because the first one is </.well-known/core>;ct=40
			try {
				
				String[] parameters = resources[i].split(";");
				
				if(parameters.length > 0 && parameters[0].split("<").length > 1) {
					String path = parameters[0].split("<")[1].replace(">", "");
					//System.out.println("PATH "+ path);
					String name = path.replace("/", "");
					
					String info = parameters[1]+";"+parameters[2];
					
					boolean obs = false;
					if(responseText.contains("obs")) {
						obs = true;
					}
					
					if (name.contains("flag")) {
						Flag newFlag = new Flag(name, path, addr.getHostAddress(), obs);
						Application.getSharedInstance().getFlagsMap().put(name, newFlag);
						System.out.println("\n"+name+" registered");
					} else {
						TrackLimit newTracklimit = new TrackLimit(name, path, addr.getHostAddress(), obs);
						Application.getSharedInstance().getTracklimitsMap().put(name, newTracklimit);
						System.out.println("\n"+name+" registered");
//						if(obs==true) {
//						Interface.observedResources.put(name, new ObservingCoapClient(newRes));	
//						Interface.observedResources.get(name).startObserving();
//					}
					}
					
					
					

				}
				
			}catch(Exception e) {
				e.printStackTrace();
				System.err.println(e);
			}
		}
		
	}
	
}
