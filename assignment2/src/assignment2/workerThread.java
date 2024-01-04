package assignment2;
import java.util.*;

public class workerThread extends Thread 
{
    int number;
    int status; // can be -2, -1, 0, 1, it communicates with main //
    int size; // size of numbers that each thread has proccessed //
    ArrayList<Integer> result;  // 2d array that keeps the value of each number and if it is prime or not //
    ArrayList<Integer> numbers;
    int pos; // indicates the number of worker //
    Object monitor = new Object();

    public workerThread(int i)
    {
    	number = -1;
    	status = 1;
    	result = new ArrayList<>();
    	numbers = new ArrayList<>();
    	size = 1;
    	pos = i;
    }
    
    @Override
    public void run()
    {
    	synchronized (hw3_2.print) 
    	{
            System.out.print("Thread ");
            System.out.print(pos);
            System.out.print(" created\n");
		}

        
        while(true)
        {
        	try 
        	{
				synchronized (hw3_2.work)
				{
					hw3_2.available_threads++;	// +1 available //
					hw3_2.work.wait();
					hw3_2.available_threads--;
				}
			} 
        	catch (Exception e) 
        	{
				e.printStackTrace();
			}
        	
        	if(status == -1)
        	{
        		break;
        	}

//        	synchronized(hw3_2.check_number)
//        	{
//        		if(hw3_2.global_number > 0)
//        		{
//            		number = hw3_2.global_number;
//            		status = 0;	// make him busy //
//            		hw3_2.global_number = -1;
//        		}
//        		else
//        		{
//        			continue;
//        		}
//
////        		System.out.println(number);
//        	}
        	numbers.add(number);
  	
        	result.add(find_prime(number));
        	
//        	try {
//				Thread.sleep(0, 1);
//			} catch (InterruptedException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
        	
        	synchronized (hw3_2.get_number)
        	{
        		hw3_2.get_number.notify();
			}
        	
        	synchronized(hw3_2.main_monitor)
        	{
        		status = 1;
        		hw3_2.main_monitor.notify();
        	}  	
        }
        status = -2;
    	synchronized(hw3_2.main_monitor)
    	{
    		hw3_2.main_monitor.notify();
    	}
    	synchronized (hw3_2.print) 
    	{
    		System.out.println("Thread " + pos + " is exiting!");
		}
    	
        // up to term //
    }

    private int find_prime(int number)
    {
        int i, flag = 0;
        if (number == 0 || number == 1)
        	flag = 1;

        for (i = 2; i <= number / 2; ++i) 
        {
            if (number % i == 0) 
            {
                flag = 1;
                break;
            }
        }
         
        return flag; 
    }
}
