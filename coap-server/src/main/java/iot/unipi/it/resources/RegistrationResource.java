package iot.unipi.it.resources;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.util.Map;

import iot.unipi.it.model.Flag;
import iot.unipi.it.model.Resource;
import iot.unipi.it.model.TrackLimit;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.server.resources.CoapExchange;

import iot.unipi.it.Application;
import iot.unipi.it.ObservableCoapClient;

public class RegistrationResource extends CoapResource {

	public RegistrationResource(String name) {
		super(name);
	}
	
	public void handleGET(CoapExchange exchange) {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		
		exchange.accept();
		InetAddress addr = exchange.getSourceAddress();
		CoapClient client = new CoapClient("coap://[" + addr.getHostAddress() + "]:5683/.well-known/core");
		CoapResponse response = client.get();
		String code = response.getCode().toString();
		if(!code.startsWith("2")) {
			System.err.println("Error with code: " + code);
			return;
		}
		
		// Let's get the node ID
		int id = fromPayloadToInt(exchange.getRequestText());
		
		String responseText = response.getResponseText();
				
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
						String flagName = "flag"+id;
						Flag newFlag = new Flag(flagName, path, addr.getHostAddress(), obs);
						Application.getSharedInstance().getFlagsMap().put(name, newFlag);
						System.out.println("\n"+name+" registered\n");
						System.out.print(">> ");
					} else {
						String tlName = "tl"+id;
						TrackLimit newTracklimit = new TrackLimit(tlName, path, addr.getHostAddress(), obs);
						Application.getSharedInstance().getTracklimitsMap().put(name, newTracklimit);
						System.out.println("\n"+name+" registered\n");
						System.out.print(">> ");
						if(obs==true) {
							Map<String, ObservableCoapClient> observableClients =  Application.getSharedInstance().getObservableClients();
							observableClients.put(name, new ObservableCoapClient(newTracklimit));
						}
					}
					
					
					

				}
				
			}catch(Exception e) {
				e.printStackTrace();
				System.err.println(e);
			}
		}
		
	}
	
	public int fromPayloadToInt(String p) {
		
		char [] a = p.toCharArray();
		return Character.getNumericValue(a[0]);
		
	}
	
}
