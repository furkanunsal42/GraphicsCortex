using ipclibcs;
using System;
using System.Diagnostics;
using System.Linq;
using System.Threading;

class color
{
    byte red, green, blue;
}

public class Program
{
    public static void main_parent()
    {
        Console.WriteLine("I'm C#");

        Pipe pipe_read = new Pipe();
        Pipe pipe_write = new Pipe();
        Event event_external = new Event();
        Event event_internal = pipe_write.get_io_event();

        Console.WriteLine("Parent: rp:" + pipe_read + " wp:" + pipe_write + " event:" + event_internal.get_handle_int() + " event_extnernal:" + event_external);

        string command = "" + 
            event_internal.get_handle_int() + " " + 
            event_external.get_handle_int() + " " + 
            pipe_write.get_read_handle_int() + " " + 
            pipe_write.get_read_handle_int();
        
        Process process = new Process();
        process.StartInfo.FileName = "GraphicsCortexApp.exe";
        process.StartInfo.Arguments = command;
        process.StartInfo.UseShellExecute = true;
        process.StartInfo.WindowStyle = ProcessWindowStyle.Normal;
        process.StartInfo.CreateNoWindow = false;
        process.StartInfo.RedirectStandardOutput = false;
        process.StartInfo.RedirectStandardInput = false;
        process.Start();

        pipe_write.set_event_trigger_condition(Pipe.IOEventTriggerCondition.WRITE);

        string message = "hello";
        pipe_write.write(message.ToArray(), (size_t)message.Length * sizeof(char), 0, "");

        byte[] message_incoming = new byte[5];

        Thread.Sleep(500);
        pipe_write.read(message_incoming, (size_t)message_incoming.Length * sizeof(byte), 0, "");
        for (int i = 0; i < message_incoming.Length; i++)

            Console.Write((char)message_incoming[i]);
        Console.Read();
    }

    public static void main_child(string[] args)
    {
        Console.WriteLine("I'm C#");

        size_t external_event_h = size_t.Parse(args[0]);
        size_t event_h = size_t.Parse(args[1]);
        size_t pipe_read_h = size_t.Parse(args[2]);
        size_t pipe_write_h = size_t.Parse(args[3]);

        Pipe pipe_read = new Pipe(0, pipe_read_h, 0);
        Pipe pipe_write = new Pipe(pipe_write_h, 0, event_h);
        Event event_external = new Event(external_event_h);

        Console.WriteLine("Child: rp:" + pipe_read + " wp:" + pipe_write + " event:" + event_h + " event_extnernal:" + event_external);

        pipe_write.set_event_trigger_condition(Pipe.IOEventTriggerCondition.WRITE);

        char[] message = new char[] { 'h', 'e', 'l', 'l', 'o' };
        pipe_write.write(message, (size_t)message.Length * sizeof(char), 0, "");

        byte[] incoming_message = new byte[5];
        pipe_read.read(incoming_message, (size_t)incoming_message.Length * sizeof(byte), 0, "");

        for (int i = 0; i < incoming_message.Length; i++)
            Console.Write((char)incoming_message[i]);
        Console.WriteLine();
        
        Console.Read();
    }

    public static void Main(string[] args)
    {
        if (args.Length == 0) main_parent();
        else main_child(args);
    }
}