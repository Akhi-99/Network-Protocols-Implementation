//SERVER

import java.net.*;
import java.io.*;

public class Server
{
	//initialize socket and input stream
	private Socket s= null;
	private ServerSocket serv_sock= null;
	private DataInputStream inp = null;
	//private DataOutputStream out = null;

	public Server(int port)
	{
		try
		{
			serv_sock= new ServerSocket(port);
			System.out.println("Server Started");

			System.out.println("Waiting for Connections");

			s= serv_sock.accept();
			System.out.println("Client accepted");

			inp = new DataInputStream(
				new BufferedInputStream(s.getInputStream()));

			String line ="";

			while(!line.equals("Over"))
			{
				try
				{
					line = inp.readUTF();
					System.out.println(line);
				}
				catch(IOException i)
				{
					System.out.println(i);
				}
			}

			System.out.println("Closing Connection");

			//close connection
			s.close();
			inp.close();
		}
		catch(IOException i)
		{
				System.out.println(i);
		}
		
	}
	public static void main(String args[])
	{
		Server ser= new Server(5000);
	}



}
//--------------------------------------------------------------------------------------------------------------------------------------

//CLIENT

import java.net.*;
import java.io.*;

public class Client
{
	//initialize socket and input stream\
	private Socket s= null;
	private DataInputStream inp = null;
	private DataOutputStream out =null;

	//constructor
	public Client(String addr,int port)
	{
		//establish conn
		try
		{
			s= new Socket(addr,port);
			System.out.println("Connected");

		//take input from terminal
			inp = new DataInputStream(System.in);

		//sends o/p to socker
			out = new DataOutputStream(s.getOutputStream());
		}

		catch(UnknownHostException u)
		{
			System.out.println(u);
		}
		catch(IOException i)
		{
			System.out.println(i);
		}

		//string to read msgz from input
		String line ="";

		while(!line.equals("Over"))
		{
			try
			{
				line = inp.readLine();
				out.writeUTF(line);
			}
			catch(IOException i)
			{
				System.out.println(i);
			}
		}

		//close connection

		try
		{
			inp.close();
			out.close();
			s.close();
		}
		catch(IOException i)
		{
			System.out.println(i);
		}

	}

	public static void main(String args[])
	{
		Client cli = new Client("127.0.0.1",5000);
	}
}