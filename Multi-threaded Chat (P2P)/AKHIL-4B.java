//SERVER
import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;

public class Server
{
	public static void main(String args[]) throws IOException
	{
		ServerSocket ss= new ServerSocket(5056);

		while(true)
		{
			Socket s=null;
			try
			{
				s= ss.accept();
				System.out.println("A new Client connected");
				//getting i/p and o/p streams
				DataInputStream dis = new DataInputStream(s.getInputStream());
				DataOutputStream dos = new DataOutputStream(s.getOutputStream());

				//creating a new thread
				Thread t= new ClientHandler(s,dis,dos);
				//invoke start 
				t.start();
			}
			catch(Exception e)
			{
				s.close();
				e.printStackTrace();
			}
		}
	}
}

//Client Handler Class

class ClientHandler extends Thread
{
	DateFormat fordate = new SimpleDateFormat("yyyy/MM/dd");
	DateFormat fortime = new SimpleDateFormat("hh:mm:ss");
	final DataInputStream dis;
	final DataOutputStream dos;
	final Socket s;

	//Constructor
	public ClientHandler(Socket s,DataInputStream dis,DataOutputStream dos)
	{
		this.s=s;
		this.dis = dis;
		this.dos = dos;
	}

	@Override
	public void run()
	{

		String received;
		String toreturn;
		while(true)
		{
			try
			{
				dos.writeUTF("What do u want?[Date|Time]..\n"+
					"Type Exit to terminate connection..\n");

				received= dis.readUTF();

				if(received.equals("Exit"))
				{
					System.out.println("Client "+this.s+"sends exit");
					System.out.println("Closing connection.");
					this.s.close();
					System.out.println("connection closed");
					break;
				}

				Date date = new Date();

				switch(received)
				{
					case "Date" :
						toreturn = fordate.format(date);
						dos.writeUTF(toreturn);
						break;
					case "Time" :
						toreturn = fortime.format(date);
						dos.writeUTF(toreturn);
						break;
					default :
						dos.writeUTF("Invalid input");
						break;
				}

			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}

		//closing resources
		try
		{
			this.dis.close();
			this.dos.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
}

--------------------------------------------------------------
//Client

import java.io.*;
import java.net.*;
import java.util.*;

public class Client
{
	public static void main(String args[]) throws IOException
	{
		try
		{
			Scanner scn = new Scanner(System.in);

			InetAddress ip = InetAddress.getByName("localhost");
			Socket s= new Socket(ip,5056);

			DataInputStream dis = new DataInputStream(s.getInputStream());
			DataOutputStream dos = new DataOutputStream(s.getOutputStream());
			
			while(true)
			{
				System.out.println(dis.readUTF());
				String tosend = scn.nextLine();
				dos.writeUTF(tosend);

				if(tosend.equals("Exit"))
				{
					System.out.println("Closing conn");
					s.close();
					System.out.println("Conn closed");
					break;
				}

				String received = dis.readUTF();
				System.out.println(received);
			}

			//closing 
			scn.close();
			dis.close();
			dos.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}


	}
}