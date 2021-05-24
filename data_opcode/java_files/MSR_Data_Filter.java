//package dataFilters;
/*
 * Created by: Alan Wang 
 * 
 * 
 * 
 * This filters data from the scanmsr command from nautilus, it will create a new file which contains the model specific registers, then the values contained in the registers along with general data
 * 
 * You must specify the input and output file paths. 
 * 
 */


import java.util.*;
import java.io.*;

public class MSR_Data_Filter {
	static ArrayList<String> MSR = new ArrayList<String>(); 
	static ArrayList<String> values = new ArrayList<String>(); 
	public static void main(String args[]) throws IOException {
		//input
		BufferedReader f = new BufferedReader(new FileReader("testrun_3.txt")); StringTokenizer input; 
		
		//output
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("filteredrun_3.txt"))); 
		
		//Filter 
		//If it has tag returned it goes into values
		//if it has tag important it goes into MSR and values
		//others are ignored
		String temp;
		temp = f.readLine(); 
	    while(temp != null) {
	    	input = new StringTokenizer(temp);
	    	String tag = ""; 
	    	if(temp.length() > 0)
	    		tag = input.nextToken(); 
	    	if(tag.equals("RETURNED----")) {
	    		values.add(temp); 
	    	}
	    	else if(tag.equals("IMPORTANT---")) {
	    		values.add(temp);
	    		MSR.add(temp); 
	    	}
			temp = f.readLine();	
	    }
		
	    
	    //output
	    out.println("Total MSRs found = " + MSR.size());
	    out.println("\n------------------------------------------------------------\n");
	    for(String item : MSR) {
	    	out.println(item);
	    }
	    out.println("\n------------------------------------------------------------\n");
	    for(String item : values) {
	    	out.println(item);
	    	input = new StringTokenizer(item); 
	    	if(input.nextToken().equals("IMPORTANT---")) {
	    		out.println(); 
	    	}
	    }
	    out.println("\n------------------------------------------------------------\n");
	    
	    
	    out.close(); 
	    f.close();
	}
}

