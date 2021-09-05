package iot.unipi.it;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.coap.MediaTypeRegistry;

import iot.unipi.it.model.Flag;
import iot.unipi.it.model.Resource;
import iot.unipi.it.model.TrackLimit;

public class Application {
	
	
	private static Application sharedInstance = new Application();
	
	public static Application getSharedInstance() {
		return sharedInstance;
	}
	
	private Map<String, TrackLimit> tracklimitsMap = new HashMap<String, TrackLimit>();
	private Map<String, Flag> flagsMap = new HashMap<String, Flag>();
	
	public Map<String, TrackLimit> getTracklimitsMap() {
		return tracklimitsMap;
	}
	
	public Map<String, Flag> getFlagsMap() {
		return flagsMap;
	}
	

	public static void main(String[] args) {
		Application.getSharedInstance().start();
	}
	
	private void start() {
		System.out.println("Welcome");
		runServer();
		System.out.println("Server started");
		System.out.println("Registration...");
		
		showMenu();
		while(true) {
			int cmd = getCommand();
			switch(cmd) {
				case 1:
					showAvailableTracklimits();
					showMenu();
					break;
				case 2:
					showAvailableFlags();
					showMenu();
					break;
				case 3:
					checkFlagColor();
					showMenu();
					break;
				case 4:
					changeFlagColor();
					showMenu();
					break;
				case 0:
					System.out.println("Leaving the Application...");
					System.out.println("Bye.");
					System.exit(0);
					break;
				default:
					showMenu();
					break;
			}
		}
	}
	
	private void runServer() {
			
		new Thread() {
			public void run() {
				Server s = new Server();
				s.startServer();
			}
		}.start();
	}
	
	
	// Menu stuff
	
	private void showAvailableTracklimits() {
		if (tracklimitsMap.size() == 0) {
			System.out.println("No Available Track Limit Sensors");
			return;
		}
		System.out.println("Available Track Limit Sensors: ");
		for(Map.Entry<String,TrackLimit> entry : tracklimitsMap.entrySet()) {
			System.out.println(entry.getValue().toString());
		}
	}
	
	private void showAvailableFlags() {
		if (flagsMap.size() == 0) {
			System.out.println("No Available Flags");
			return;
		}
		System.out.println("Available Track Flags: ");
		for(Map.Entry<String,Flag> entry : flagsMap.entrySet()) {
			System.out.println(entry.getKey() + ": " + entry.getValue().toString());
		}
	}
	
	private void checkFlagColor() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Insert the flag name: ");
		System.out.print(">> ");
		String name;
		try {
			name = input.readLine();
			Flag flag = this.flagsMap.get(name);
			if(flag == null) {
				System.err.println("No flag with this name exists");
				return;
			}
			CoapClient flagClient = new CoapClient(flag.getCoapURI());
			CoapResponse response = flagClient.get();
			String code = response.getCode().toString();
			if(!code.startsWith("2")) {
				System.err.println("Error:"+code);
				return;
			}
			System.out.println("The color of the flag is " + response.getResponseText());
		} catch (IOException e) {
			System.err.println("Error: " + e.getMessage());
		}
	}
	
	private void changeFlagColor() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Insert the flag name: ");
		System.out.print(">> ");
		String name;
		try {
			name = input.readLine();
			Flag flag = this.flagsMap.get(name);
			if(flag == null) {
				System.err.println("No flag with this name exists");
				return;
			}
			System.out.println("Insert the flag color: ");
			System.out.println(">> ");
			String flagColor = input.readLine();
			if (!this.isValidFlag(flagColor)){
				System.out.println("Error: the flag color is not valid.");
				return;
			}
			String postBody = "flag=" + flagColor;
			if(flagColor.equalsIgnoreCase("yellow")) {
				System.out.println("Insert the seconds: ");
				System.out.println(">> ");
				int seconds = getCommand();
				if (seconds != -1) {
					postBody += "&seconds=" + seconds;
				} else {
					postBody += "&seconds=10";
				}
			} else {
				postBody += "&seconds=10";
			}
			CoapClient flagClient = new CoapClient(flag.getCoapURI());
			CoapResponse response = flagClient.post(postBody, MediaTypeRegistry.TEXT_PLAIN);
			String code = response.getCode().toString();
			if(!code.startsWith("2")) {
				System.err.println("Error:"+code);
				return;
			}
		} catch (IOException e) {
			System.err.println("Error: " + e.getMessage());
		}
	}
	
	private int getCommand() {
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
		String line;
		int intCommand;
		try {
			line = input.readLine();
			intCommand = Integer.parseInt(line);
			return intCommand;
		} catch (IOException e) {
			return -1;
		} catch (NumberFormatException n) {
			return -1;
		}
	}
	
	public void showMenu() {
		System.out.print("\nPlease, insert a command: \n"
				+ "1 - Show Registered Track Limit Sensors.\n"
				+ "2 - Show Registered Flags\n"
				+ "3 - Check Flag Color\n"
				+ "4 - Set flag color\n"
				+ "5 - Start Observing\n"
				+ ""
				+ "0 - Exit.\n"
				+ "\n"
				+ ">> ");
	}
	
	private boolean isValidFlag(String flag) {
		if(flag.equalsIgnoreCase("red")) {
			return true;
		}
		if(flag.equalsIgnoreCase("yellow")) {
			return true;
		}
		if(flag.equalsIgnoreCase("green")) {
			return true;
		}
		return false;
	}

}