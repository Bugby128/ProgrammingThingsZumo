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

        SerialPort port;

        public Form1()
        {
            InitializeComponent();

            port = new SerialPort("COM5", 9600);
            port.Open();
        }

        private void buttonLEFT_Click(object sender, EventArgs e)
        {
            port.Write("L");
        }

        private void buttonRIGHT_Click(object sender, EventArgs e)
        {
            port.Write("R");
        }

        private void buttonUP_Click(object sender, EventArgs e)
        {
            port.Write("U");
        }

        private void buttonDOWN_Click(object sender, EventArgs e)
        {
            port.Write("D");
        }

        private void buttonSTOP_Click(object sender, EventArgs e)
        {
            port.Write("S");
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            port.Close();
        }
    }
}
