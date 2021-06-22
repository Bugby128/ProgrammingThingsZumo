﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace ProgrammingThingsGUI
{
    public partial class Form1 : Form
    {
        SerialPort port;    //Creates a variable to hold the serial port settings

        public Form1()
        {
            InitializeComponent();

            port = new SerialPort("COM5", 9600);    //Sets up the port for my connected USB XBee
            port.Open();    //Opens the port to allow for communication between the GUI and the Zumo
            port.DataReceived += new SerialDataReceivedEventHandler(ZumoMessage);   //This creates an event handler, this waits for the data to be recieved from the zumo
        }

        //This button starts up the robots automatic navigation through the corridor
        private void buttonGO_Click(object sender, EventArgs e)
        {
            port.Write("G");
        }

        //This button continues the robots automatic navigation after it has been instructed to turn a corner
        private void buttonCONTINUE_Click(object sender, EventArgs e)
        {
            port.Write("C");
            textBox1.Text = ""; //Resets the text box after the Zumo robot continues auto control
        }

        //This button rotates the robot 90 degrees anti-clockwise 
        private void buttonLEFT_Click(object sender, EventArgs e)
        {
            port.Write("L");
        }

        //This button rotates the robot 90 degrees clockwise 
        private void buttonRIGHT_Click(object sender, EventArgs e)
        {
            port.Write("R");
        }

        private delegate void SetTextDeleg(string text);    //The delegate is used to transfer the data from the event handler thread, to the main ui thread so it can be used in the textbox

        //This function is run whenever data is received from the zumo
        void ZumoMessage(object sender, SerialDataReceivedEventArgs e)
        {
            string data = port.ReadLine();  //Reads the data sent from the zumo and creates a string variable to hold it
            this.BeginInvoke(new SetTextDeleg(PopulateTextbox), new object[] { data }); //This calls the PopulateTextbox function, using the delegate to allow the textbox to be populated with the value the zumo sent
        }

        private void PopulateTextbox(string data) 
        { 
            textBox1.Text = data.Trim(); //Sets the texbox's value to the data received from the zumo
        }  

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            port.Close();
        }

        
    }
}
