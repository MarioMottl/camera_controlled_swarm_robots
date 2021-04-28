using System;
using System.Configuration;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Net.Sockets;
using System.Windows.Forms;
using vis1.Properties;
using ZedHL;
using ZedGraph;
using System.Windows.Media;
using Brushes = System.Windows.Media.Brushes;

namespace vis1
{
    partial class VisForm3
    {
        #region Timing
        private const double FSample = 100;
        private const double Sample = 1 / FSample;
        private const int Disp = 100;  // milliSec
        private const int Thread = 20; // milliSec
        #endregion

	    private IValueSink []_ivsBuffer = new IValueSink[10];

        private const int _communicationNone = 0;
        private const int _communicationCom = 1;
        private const int _communicationSocket = 2;

        private int _communicationType = _communicationNone;

        //Konfigurationsdialog zur Auswahl einer TCP-Verbindung oder eines COM-Ports
        bool ConfigCommunication()
        {
            if (_communicationType == _communicationNone)
            {
                SelectSrc selectSrcDialog = new SelectSrc(); // Open Source Selection Dialog
                DialogResult selectSrcResult = selectSrcDialog.ShowDialog(this);
                if(selectSrcDialog.useSocket)
                {
                    // Select IP address and port number and set BinaryReader and BinaryWriter
                    // TODO: set Protocol handler
                    SelectTCPIP selectIpDialog = new SelectTCPIP();
                    DialogResult selectIpResult = selectIpDialog.ShowDialog(this);

                    if(selectIpDialog.error)
                    {
                        // if an error occured or the input form was cancelled, return false
                        return false;
                    }

                    try
                    {
                        // Initialise an TCP Socket
                        m_socket = new TcpClient();
                        m_socket.Connect(selectIpDialog.ipAddress, selectIpDialog.port); // try to connect to given IP + Port
                    }
                    catch(Exception ex)
                    {
                        if( ex is System.Net.Sockets.SocketException)
                        {
                            // Connect failed to connect
                            MessageBox.Show(Resources.ConnectError + selectIpDialog.ipAddress + "\n" + ex.Message);
                            return false;
                        }
                    }
                    m_protocolHandler = new ProtocolHandler(m_socket, this);
                    _communicationType = _communicationSocket;
                    return true;
                }
                else if(selectSrcDialog.useCOM)
                {
                    // Select COM Port, set Binary Reader and Writer
                    var comport = "";
                    var baudrate = "";
                    var databits = "";
                    Parity parity = Parity.None;
                    StopBits stopbits = StopBits.One;
                    var comDialog = new ChooseCom();    //Open window

                    if (comDialog.ShowDialog(this) == DialogResult.OK)
                    {
                        comport = comDialog.Port;       //Save selected COM-Port
                        baudrate = comDialog.Baud;      //Save selected BAUD-Rate
                        databits = comDialog.DataBits;  //Save selected Data Bits
                        switch (comDialog.Parity)        //Save selected Parity
                        {
                            case "None":
                                parity = Parity.None;
                                break;
                            case "Odd":
                                parity = Parity.Odd;
                                break;
                            case "Even":
                                parity = Parity.Even;
                                break;
                            case "Mark":
                                parity = Parity.Mark;
                                break;
                            case "Space":
                                parity = Parity.Space;
                                break;
                        }
                        switch (comDialog.StopBits)     //Save selected Stop Bits
                        {
                            case "One":
                                stopbits = StopBits.One;
                                break;
                            case "OnePointFive":
                                stopbits = StopBits.OnePointFive;
                                break;
                            case "Two":
                                stopbits = StopBits.Two;
                                break;
                        }
                        comDialog.Dispose();
                    }

                    if (comport == "")
                    {
                        return false;
                    }

                    m_serialPort.PortName = comport;
                    m_serialPort.BaudRate = Convert.ToInt32(baudrate);
                    m_serialPort.DataBits = Convert.ToInt32(databits);
                    m_serialPort.Parity = parity;
                    m_serialPort.StopBits = stopbits;

                    try
                    {
                        m_serialPort.Open(); //Serielle verbindung öffnen

                        m_protocolHandler = new ProtocolHandler(m_serialPort, this);    //Start ProtocolHandler
                        _communicationType = _communicationCom;    //Declare Communicaton as open

                        return true;
                    }
                    catch (IOException)
                    {
                        MessageBox.Show(@"IO Exception: " + comport + Resources.PortNotFound);
                        return false;
                    }
                    catch (Exception)
                    {
                        MessageBox.Show(@"Exception: " + comport + Resources.PortNotFound);
                        return false;
                    }
                }
                else
                {
                    // no selection made
                    return false;
                }
            }
            else
            {
                // already connected
                return false;
            }
        }

	    void CloseCommunication()
	    {
            if (_communicationType == _communicationCom)
            {
                m_serialPort.Close();
                m_protocolHandler.Close();
            }
            if(_communicationType == _communicationSocket)
            {
                m_socket.GetStream().Close();
                m_socket.Close();
                m_protocolHandler.Close();
            }
            _communicationType = _communicationNone;
        }

        void SetupLineChart()
        {
            GraphElements.LineChart.GraphPane.Title.Text = "";

            GraphElements.LineChart.GraphPane.Y2Axis.IsVisible = true;

            //Apply X-Axis Values
            GraphElements.LineChart.GraphPane.XAxis.Scale.Max = GraphElements.x - ConfigObject.configuration.axis[0].min;
            GraphElements.LineChart.GraphPane.XAxis.Scale.Min = GraphElements.x - ConfigObject.configuration.axis[0].max;

            //Apply Y-Axis Values
            GraphElements.LineChart.GraphPane.YAxis.Scale.Max = ConfigObject.configuration.axis[1].max;
            GraphElements.LineChart.GraphPane.YAxis.Scale.Min = ConfigObject.configuration.axis[1].min;

            //Apply Y2-Axis Values
            GraphElements.LineChart.GraphPane.Y2Axis.Scale.Max = ConfigObject.configuration.axis[2].max;
            GraphElements.LineChart.GraphPane.Y2Axis.Scale.Min = ConfigObject.configuration.axis[2].min;

            GraphElements.LineChart.GraphPane.XAxis.Title.Text = "";
            GraphElements.LineChart.GraphPane.YAxis.Title.Text = "";

            zedGraphLineChart.AxisChange();

            //Create Curves and load Settings from app.config
            for (var track = 0; track < 9; track++)    
            {
                GraphElements.LineChart.GraphPane.AddCurve(ConfigObject.configuration.channels[track].name, 
                    new double[0],
                    new double[0],
                    System.Drawing.Color.FromName(ConfigObject.configuration.channels[track].color),
                    SymbolType.None);
                GraphElements.LineChart.GraphPane.CurveList[track].IsY2Axis = ConfigObject.configuration.channels[track].IsY2;
            }
        }

        void SetupBarChart()
        {
            GraphElements.BarChart.GraphPane.Title.Text = "";

            GraphElements.BarChart.GraphPane.Y2Axis.IsVisible = true;

            //Apply Y-Axis Values
            GraphElements.BarChart.GraphPane.YAxis.Scale.Max = ConfigObject.configuration.axis[1].max;
            GraphElements.BarChart.GraphPane.YAxis.Scale.Min = ConfigObject.configuration.axis[1].min;

            //Apply Y2-Axis Values
            GraphElements.BarChart.GraphPane.Y2Axis.Scale.Max = ConfigObject.configuration.axis[2].max;
            GraphElements.BarChart.GraphPane.Y2Axis.Scale.Min = ConfigObject.configuration.axis[2].min;

            GraphElements.BarChart.GraphPane.XAxis.Scale.Max = 0.5;
            GraphElements.BarChart.GraphPane.XAxis.Scale.Min = -0.5;

            GraphElements.BarChart.GraphPane.XAxis.IsVisible = false;
            GraphElements.BarChart.GraphPane.YAxis.Title.Text = "";

            zedGraphBarChart.AxisChange();

            //Create Curves and load Settings from app.config
            for (var track = 0; track < 9; track++)    
            {
                GraphElements.BarChart.GraphPane.AddBar(ConfigObject.configuration.channels[track].name,
                    null,
                    GraphElements.barData, 
                    System.Drawing.Color.FromName(ConfigObject.configuration.channels[track].color));
                GraphElements.BarChart.GraphPane.CurveList[track].IsY2Axis = ConfigObject.configuration.channels[track].IsY2;
            }
        }

        //TODO: Slider Wert senden
        public void OnValChanged(int aId, MSlider aSlider) // SliderCB  //Slider Wert senden
        {

        }

        public void OnMoseUp(int aId, MSlider aSlider) // SliderCB
        {
        }
    }
}