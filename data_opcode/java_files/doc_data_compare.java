//package dataFilters;
import java.io.*;
import java.util.*;

/*
 * 
 * 
 * Used to compare the filtered doc data and our filtered data for MSRs and sort it into 3 categories, documented and detected, undocumented but detected (Interesting ones), and documented but undetected 
 * 
 * 
 */

public class doc_data_compare {
	static ArrayList<String> msrData = new ArrayList<String>();
	static ArrayList<String> msrDoc = new ArrayList<String>(); 
	
	static int doc_size; 
	public static void main(String args[]) throws IOException {
		//input
		BufferedReader fDoc = new BufferedReader(new FileReader("../filtered_doc_msrs.txt")); StringTokenizer input;
		BufferedReader fData = new BufferedReader(new FileReader("../filteredrun_3.txt"));
		
		//output
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("../compare_final_msr.txt"))); 

		//comparing 
		//read in MSR addresses from each file and compare 
		//reading in data files 
		fData.readLine();
		fData.readLine();
		fData.readLine();
		fData.readLine(); //offset
		input = new StringTokenizer(fData.readLine()); 
		while(input.hasMoreTokens()) {
			input.nextToken(); input.nextToken(); input.nextToken();
			String address = input.nextToken(); 
			address = address.substring(2, address.length()); //cut out the 0x  
			msrData.add(address.toUpperCase()); //since it has the leading 0s we need to add the leading 0s to msrDoc address in order to compare
			assert(address.length() == 8); //sanity check 
			
			input = new StringTokenizer(fData.readLine()); 
		}
		
		//reading in doc MSR addresses 
		fDoc.readLine();
		fDoc.readLine();
		input = new StringTokenizer(fDoc.readLine());
		while(input.hasMoreTokens()) {
			String address = input.nextToken(); 
			address = addZeroes(address); //need to add leading zeros
			msrDoc.add(address.toUpperCase()); 
			assert(address.length() == 8); //sanity check 
			
			String temp = fDoc.readLine(); //this is not the best way to do this, but it is the easiest after setting everything up already the wrong way 
			if(temp == null)
				input = new StringTokenizer(""); 
			else 
				input = new StringTokenizer(temp);
		}
		doc_size = msrDoc.size(); 
		
		//start comparing the two numbers and sorting into the 2 different groups
		//We are just going to brute force it, which takes O(n^2) time but since n is only around 400 (200 in doc + 200 in data) it makes little sense to make it any faster  
		ArrayList<String> documented_detected = new ArrayList<String>(); //boring 
		ArrayList<String> undocumented_detected = new ArrayList<String>(); //interesting 
		ArrayList<String> documented_undetected = new ArrayList<String>(); //weird 
		
		for(String data : msrData) {
			if(msrDoc.contains(data)) 
				documented_detected.add(data); //It is in doc and data
			else 
				undocumented_detected.add(data); //We detected it but it isn't documented
			
			//remove it from doc 
			//Makes it easier to find documented MSRs that we didn't detected 
			int rem = msrDoc.indexOf(data);
			if(rem != -1) 
				msrDoc.remove(rem); 
		}
		documented_undetected = msrDoc; 
		
		
		
		//output 
		//General information 
		out.println("Total MSRs found on machine: " + msrData.size() + "\tTotal MSRs documented by Intel: " + doc_size);
		out.println("Total MSRs documented and detected on machine (boring): " + documented_detected.size()); 
		out.println("Total MSRs undocumented and detected on machine (interesting): " + undocumented_detected.size());
		out.println("Total MSRs documented and not detected on machine (weird): " + documented_undetected.size());
		
		//MSR address for each category 
		//Doc and detected 
		out.println("\nDocumented and Detected");
		for(String item : documented_detected) {
			out.println("0x" + item);
		}
		
		//undoc and detected 
		out.println("\nUndocumented and Detected");
		for(String item : undocumented_detected) {
			out.println("0x" + item);
		}
		
		out.println("\nDocumented and Undetected");
		for(String item : documented_undetected) {
			out.println("0x" + item);
		}
		
		out.close();
		fDoc.close();
		fData.close();
	}
	
	//helper functions
	public static String addZeroes(String a) {
		String ret = ""; 
		for(int i = 0; i < 8-a.length(); i++) {
			ret += "0"; 
		}
		ret += a; 
		return ret; 
	}
	
}

