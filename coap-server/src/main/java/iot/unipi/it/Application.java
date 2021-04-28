package iot.unipi.it;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

import iot.unipi.it.model.Resource;

public class Application {
	
	private static Map<String, Resource> resourceMap = new HashMap<String, Resource>();
	
	public static Map<String, Resource> getResourceMap() {
		return resourceMap;
	}

	public static void main(String[] args) {
		
		System.out.println("Welcome");
		
		System.out.println("Registration...");
		
		
		//runServer();
		
		showMenu();
		
		while(true) {
			int cmd = getCommand();
			switch(cmd) {
				case 1:
					showAvailableResources();
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
	
	public static void runServer() {
			
		new Thread() {
			public void run() {
				Server s = new Server();
				s.startServer();
			}
		}.start();
	}
	
	
	// Menu stuff
	
	private static void showAvailableResources() {
		if (resourceMap.size() == 0) {
			System.out.println("No Available Resources");
			return;
		}
		System.out.println("Available Resources: ");
		for(Map.Entry<String,Resource> entry : resourceMap.entrySet()) {
			System.out.println(entry.getValue().toString());
		}
	}
	
	private static int getCommand() {
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
	
	public static void showMenu() {
		System.out.print("\nPlease, insert a command: \n"
				+ "1 - Show Registered Resources.\n"
				+ "0 - Exit.\n"
				+ "\n"
				+ ">> ");
	}

}