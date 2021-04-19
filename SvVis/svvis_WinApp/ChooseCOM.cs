using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace vis1
{
    public partial class ChooseCom : Form
    {
        public string Port = "";
        public string Baud = "";
        public string DataBits = "";
        public string Parity;
        public string StopBits;
        public ChooseCom()
        {
            InitializeComponent();

            selectBAUD.SelectedIndex = 2;
            selectDataBits.SelectedIndex = 4;
            selectParity.SelectedIndex = 0;
            selectStopBits.SelectedIndex = 0;

            SetPorts();
            ThemeHandler.ApplyTheme(this);
        }

        private void SetPorts() //Display all available Ports
        {
            PortsLB.Items.Clear();
            PortsLB.Items.AddRange( SerialPort.GetPortNames() );
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (PortsLB.SelectedIndex == -1)
            {
                MessageBox.Show("Please select an Item first!");    
            }
            else
            {
                Port = PortsLB.SelectedItem.ToString();             //Save selected COM-Port
                Baud = selectBAUD.SelectedItem.ToString();          //Save selected BAUD-Rate
                DataBits = selectDataBits.SelectedItem.ToString();  //Save selected Data Bits
                Parity = selectParity.SelectedItem.ToString();      //Save selected Parity
                StopBits = selectStopBits.SelectedItem.ToString();  //Save selected Stop Bits
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
            this.Close();
        }
    }
}
