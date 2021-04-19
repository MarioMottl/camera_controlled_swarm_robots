using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace vis1
{
    public partial class ChooseCom : Form
    {
        public string Port = "";

        public ChooseCom()
        {
            InitializeComponent();

            SetPorts();
        }

        private void SetPorts()
        {
            PortsLB.Items.Clear();
            PortsLB.Items.AddRange(SerialPort.GetPortNames());
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (PortsLB.SelectedIndex == -1)
            {
                MessageBox.Show(@"Please select an Item first!");    
            }
            else
            {
                Port = PortsLB.SelectedItem.ToString();
                AcceptButton = buttonOK;
            }
        }

        private void buttonRefresh_Click(object sender, EventArgs e)
        {
            SetPorts();
        }

        private void PortsLB_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }
    }
}
