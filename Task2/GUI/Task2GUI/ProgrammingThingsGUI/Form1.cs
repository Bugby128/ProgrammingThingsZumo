using System;
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
        }

        //This button starts up the robots automatic navigation through the corridor
        private void buttonGO_Click(object sender, EventArgs e)
        {
            port.Write("G");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            port.Close();
        }
    }
}
