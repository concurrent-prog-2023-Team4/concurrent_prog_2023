package assignment2;
import java.io.*;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Hello world!
 *
 */


public class hw3_2
{
	public static int global_number = -1;
	public static int available_threads = 0;
	public static int didNotify = 0;
	
	public static Object main_monitor = new Object();
	public static Object work = new Object();
	public static Object get_number = new Object();
	public static Object print = new Object();
	public static Object check_number = new Object();
	
    public static void main( String[] args )
    {
        // if(args.length != 2)
        // {
        //     System.exit(0);
        // }
    	hw3_2 program = new hw3_2();
    	program.execute(args);
        
    }
    
    public void execute(String[] args)
    {
        String Filepath = args[1];
        int num_threads = Integer.parseInt(args[0]);
//        available_threads = num_threads;
        workerThread[] threads = new workerThread[num_threads];
        int available = 0;
        
        for(int i = 0; i < num_threads; i++)
        {
        	threads[i] = new workerThread(i);
        }
        for(int i = 0; i < num_threads; i++)
        {
        	threads[i].start();
        }

        try (BufferedReader br = new BufferedReader(new FileReader(Filepath))) 
        {
            String line;
            while ((line = br.readLine()) != null) 
            {
                // Process each line as needed
            	if(Integer.parseInt(line) < 0)
            	{
            		break;
            	}
//            	synchronized (work)
//            	{
            		global_number = Integer.parseInt(line);            		
//            	}

            	
            	synchronized (work) 
            	{
            		available = available_threads;
				}

            	if(available == 0)	// all busy //
            	{
            		synchronized(main_monitor)
            		{
//                			blocked_main = 1;
            			try 
            			{
							main_monitor.wait();
						} 
            			catch (InterruptedException e) 
            			{
							e.printStackTrace();
						}

            		}
            	}
            	synchronized (work) 
            	{
            		available = available_threads;
				}

        		if(available > 0)	// available //
        		{
        			synchronized (work) 
        			{
	
//        				System.out.println("Find available!");
						work.notify();
					}
        			
        			synchronized (get_number) 
        			{
        				//System.out.println("Main");
        				try 
        				{
//        					if(didNotify != 1)
//        					{
        						get_number.wait();	
        						
//        					}
//        					else
//        					{
//        						didNotify = 0;
//        					}
        						
						} 
        				catch (InterruptedException e) 
        				{
							e.printStackTrace();
						}
					}
        		}
            }
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }
        
        // notify them to finish //
        for(int i = 0; i < num_threads; i++)
        {
        	if (threads[i].status == 0)	// busy //
        	{
        		synchronized(main_monitor)
        		{
        			try 
        			{
        				if(i > 0)
        				{
        					i--;        					
        				}
        				main_monitor.wait();
//        				threads[i].monitor.wait();
					} 
        			catch (InterruptedException e) 
        			{
						e.printStackTrace();
					}
        		}
        	}
        	 // notify to terminate //
        }
        
        for(int i = 0; i < num_threads; i++)
        {
        	threads[i].status = -1;
        }
        synchronized (work) 
        {
			work.notifyAll();
		}
        
        for(int i = 0; i < num_threads; i++)
        {
        	if (threads[i].status != -2)	// not terminate //
        	{
        		synchronized(main_monitor)
        		{
        			try 
        			{
        				if(i > 0)
        				{
        					i--;        					
        				}
        				main_monitor.wait();
//        				threads[i].monitor.wait();
					} 
        			catch (InterruptedException e) 
        			{
						e.printStackTrace();
					}
        		}
        	}
        }
        
//        try {
//			Thread.sleep(1000);
//		} catch (InterruptedException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}

        int count = 0;
        try (FileWriter fileWriter = new FileWriter("out.txt");
                BufferedWriter bufferedWriter = new BufferedWriter(fileWriter)) {

               // Write content to the file
	            for (int i = 0; i < num_threads; i++)
	            {
	            	for(int j = 0; j < threads[i].result.size(); j++)
	            	{
	            		//if(threads[i].result != null)
	            		//{
	            		bufferedWriter.write("Number ");
	            		bufferedWriter.write(threads[i].numbers.get(j).toString());
	            		if(threads[i].result.get(j) == 1)
	            		{
	            			bufferedWriter.write(" is NOT PRIME!");
	            			bufferedWriter.newLine();
	            		}
	            		else
	            		{
	            			bufferedWriter.write(" is PRIME!");
	            			bufferedWriter.newLine();	
	            		}
	            		
	            			count++;       		        			
	            		//}
	            	}
	            }

           } catch (IOException e) {
               e.printStackTrace();
           }
        synchronized (print) 
        {
        	System.out.println("Total numbers calculated are " + count);
		}
      
       System.exit(0);
    }
}
