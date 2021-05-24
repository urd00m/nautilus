//package dataFilters;
import java.util.*;
import java.io.*;
/*
 * 
 * Created to filter out MSR addresses from Intel's MSR documentation to compare with collected data on the Intel i3-2*** 
 * 
 * 
 */
public class MSR_doc_filter {
	static ArrayList<String> MSR_Documented = new ArrayList<String>(); 
	public static void main(String args[]) throws IOException {
		//input
		BufferedReader f = new BufferedReader(new FileReader("../doc_msrs.txt")); StringTokenizer input; 
		
		//output
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("filtered_doc_msrs.txt"))); 
		
		//Filter 
		//If it has the H tag we keep it
		
		String temp;
		temp = f.readLine(); 
	    while(temp != null) {
	    	input = new StringTokenizer(temp);
	    	String tag = ""; 
	    	if(temp.length() > 0)
	    		tag = input.nextToken(); 
	    	
	    	if(tag.length() > 1 && tag.charAt(tag.length()-1) == 'H') {
	    		MSR_Documented.add(temp.substring(0, tag.length()-1)); //ignore the H 
	    	}
			temp = f.readLine();	
	    }
	    
	    out.println("Total Documented MSRs: " + MSR_Documented.size() + "\n");
	    for(String item : MSR_Documented) {
	    	out.println(item);
	    }
	    
	    out.close();
	    f.close();
	}
}

