using System;
using System.Configuration;
using System.Windows.Forms;
using System.IO.Ports;
using ZedHL;
using ZedGraph;
using System.Windows.Media;
using System.IO;
using System.Collections.Generic;
using Label = System.Windows.Forms.Label;

namespace vis1
{
    public partial class VisForm3 : Form, IPrintCB, SliderCB
    {
        #region Member Variables
        Label[] m_labels = new Label[9];
        
        ProtocolHandler m_protocolHandler;
        About about = new About();
        #endregion
        #region Decoder Thread

        bool _doGraphUpdate = true;
        #endregion    

        public VisForm3()
        {
            InitializeComponent();

            GraphElements.LineChart = zedGraphLineChart;
            GraphElements.BarChart = zedGraphBarChart;

            m_labels[0] = m_Disp1;
            m_labels[1] = m_Disp2;
            m_labels[2] = m_Disp3;
            m_labels[3] = m_Disp4;
            m_labels[4] = m_Disp5;
            m_labels[5] = m_Disp6;
            m_labels[6] = m_Disp7;
            m_labels[7] = m_Disp8;
            m_labels[8] = m_Disp9;

            SettingsElements.m_enableButtons[0] = enableChannel1;
            SettingsElements.m_enableButtons[1] = enableChannel2;
            SettingsElements.m_enableButtons[2] = enableChannel3;
            SettingsElements.m_enableButtons[3] = enableChannel4;
            SettingsElements.m_enableButtons[4] = enableChannel5;
            SettingsElements.m_enableButtons[5] = enableChannel6;
            SettingsElements.m_enableButtons[6] = enableChannel7;
            SettingsElements.m_enableButtons[7] = enableChannel8;
            SettingsElements.m_enableButtons[8] = enableChannel9;

            SettingsElements.m_channelNameLabels[0] = channelName1;
            SettingsElements.m_channelNameLabels[1] = channelName2;
            SettingsElements.m_channelNameLabels[2] = channelName3;
            SettingsElements.m_channelNameLabels[3] = channelName4;
            SettingsElements.m_channelNameLabels[4] = channelName5;
            SettingsElements.m_channelNameLabels[5] = channelName6;
            SettingsElements.m_channelNameLabels[6] = channelName7;
            SettingsElements.m_channelNameLabels[7] = channelName8;
            SettingsElements.m_channelNameLabels[8] = channelName9;

            SettingsElements.m_selectY1[0] = SetY1Ch1;
            SettingsElements.m_selectY1[1] = SetY1Ch2;
            SettingsElements.m_selectY1[2] = SetY1Ch3;
            SettingsElements.m_selectY1[3] = SetY1Ch4;
            SettingsElements.m_selectY1[4] = SetY1Ch5;
            SettingsElements.m_selectY1[5] = SetY1Ch6;
            SettingsElements.m_selectY1[6] = SetY1Ch7;
            SettingsElements.m_selectY1[7] = SetY1Ch8;
            SettingsElements.m_selectY1[8] = SetY1Ch9;

            SettingsElements.m_selectY2[0] = SetY2Ch1;
            SettingsElements.m_selectY2[1] = SetY2Ch2;
            SettingsElements.m_selectY2[2] = SetY2Ch3;
            SettingsElements.m_selectY2[3] = SetY2Ch4;
            SettingsElements.m_selectY2[4] = SetY2Ch5;
            SettingsElements.m_selectY2[5] = SetY2Ch6;
            SettingsElements.m_selectY2[6] = SetY2Ch7;
            SettingsElements.m_selectY2[7] = SetY2Ch8;
            SettingsElements.m_selectY2[8] = SetY2Ch9;

            //Load Enable-Settings into Enable Buttons
            for (int i = 1; i <= SettingsElements.m_enableButtons.Length; i++)
            {
                SettingsElements.m_enableButtons[i - 1].Checked = ConfigObject.configuration.channels[i - 1].enable;
            }

            //Load Name and Color into Name Labels
            for (int i = 1;i <= SettingsElements.m_channelNameLabels.Length; i++)
            {
                SettingsElements.m_channelNameLabels[i - 1].Text = ConfigObject.configuration.channels[i - 1].name;
                SettingsElements.m_channelNameLabels[i - 1].ForeColor = System.Drawing.Color.FromName(ConfigObject.configuration.channels[i - 1].color);
            }

            //Load Y2 Selection into Choos Axis Radiobuttons
            for (int i = 1; i <= SettingsElements.m_selectY1.Length; i++)
            {
                SettingsElements.m_selectY1[i - 1].Checked = !ConfigObject.configuration.channels[i - 1].IsY2;
            }

            //Load Y2 Selection into Choos Axis Radiobuttons
            for (int i = 1; i <= SettingsElements.m_selectY2.Length; i++)
            {
                SettingsElements.m_selectY2[i - 1].Checked = ConfigObject.configuration.channels[i - 1].IsY2;
            }

            tabPage1.Text = "Line Chart";
            tabPage2.Text = "Bar Chart";

            SetupLineChart();
            SetupBarChart();

            this.statusCheckbox.Enabled = false;
            this.Text = "SvVis " + vis1.Properties.Resources.Versionsnummer;
            menuStrip1.Renderer = new ToolStripProfessionalRenderer(new MenuColorTable());
            //ThemeHandler.ApplyTheme(this);
            if(vis1.Properties.Resources.ThemeDefault == vis1.Properties.Resources.ThemeLight)
            {
                this.lightToolStripMenuItem.PerformClick();
            }
            else if(vis1.Properties.Resources.ThemeDefault == vis1.Properties.Resources.ThemeDark)
            {
                this.darkToolStripMenuItem.PerformClick();
            }
        }

        protected override void OnLoad(EventArgs e)
        {

            _decodeTimer.Interval = Thread;

            base.OnLoad(e);
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            acqOnOffMenuItem.Checked = false;
            OnAcqOnOffMenue(null, null);

			CloseCommunication();
            base.OnFormClosing(e);
        }

        void OnAcqOnOffMenue(object sender, EventArgs e) //Toggle Acq. On/Off
        {
            if (_communicationType == _communicationCom)
            {
                if (acqOnOffMenuItem.Checked)
                {
                    m_serialPort.DiscardInBuffer();
                    System.Threading.Thread.Sleep(200);

                    m_protocolHandler.SwitchAcq(true);
                    m_protocolHandler.Flush();

                }
                else
                {
                    try
                    {
                        m_protocolHandler.SwitchAcq(false);
                        m_protocolHandler.Flush();
                    }
                    catch (NullReferenceException)
                    {
                        Console.Out.WriteLine("NullReferenceException caught!");
                    }

                    System.Threading.Thread.Sleep(200);

                }
            }
            else if(_communicationType == _communicationSocket)
            {
                if(acqOnOffMenuItem.Checked)
                {
                    System.Threading.Thread.Sleep(200);
                    m_protocolHandler.SwitchAcq(true);
                    m_protocolHandler.Flush();
                }
                else
                {
                    try
                    {
                        m_protocolHandler.SwitchAcq(false);
                        m_protocolHandler.Flush();
                    }
                    catch (NullReferenceException)
                    {
                        Console.Out.WriteLine("NullReferenceException caught!");
                    }
                    System.Threading.Thread.Sleep(200);
                }
            }
        }

        void OnEmptyReceiveBufferMenue(object sender, EventArgs e)  //Empty ReciveBuffer
        {
            if(m_serialPort.IsOpen)
            {
                m_serialPort.DiscardInBuffer();
            }
        }

        void OnClearMessagesMenue(object sender, EventArgs e)   //Clear Messages
        {
            m_logOutput.Items.Clear();
        }

        void OnDecodeTimer(object sender, EventArgs e)
        {
            try
            {
                if (m_protocolHandler.ParseAllPackets())
                {
                    DisplayValues();
                }
                    
            }
            catch(InvalidOperationException)
            {
                timerDisable();
                CloseCommunication();
                this.statusCheckbox.Enabled = false;
            }
        }

        void DisplayValues()
        {
            //Display Values numerically
            for (int i = 0; i < m_protocolHandler.NVals; i++)
            {
                m_labels[i].Text = String.Format("{0:F2}", m_protocolHandler.m_floatValues[i]);
            }
            //Apply Curves to correct Axis
            for (int i = 0; i < SettingsElements.m_selectY1.Length; i++)
            {
                GraphElements.LineChart.GraphPane.CurveList[i].IsY2Axis = !SettingsElements.m_selectY1[i].Checked;
                GraphElements.BarChart.GraphPane.CurveList[i].IsY2Axis = !SettingsElements.m_selectY1[i].Checked;
                ConfigObject.configuration.channels[i].IsY2 = !SettingsElements.m_selectY1[i].Checked;
            }
            //Apply Curves to correct Axis
            for (int i = 0; i < SettingsElements.m_selectY2.Length; i++)
            {
                GraphElements.LineChart.GraphPane.CurveList[i].IsY2Axis = SettingsElements.m_selectY2[i].Checked;
                GraphElements.BarChart.GraphPane.CurveList[i].IsY2Axis = SettingsElements.m_selectY2[i].Checked;
                ConfigObject.configuration.channels[i].IsY2 = SettingsElements.m_selectY2[i].Checked;
            }
            displayValuesBarChart();
            displayValuesLineChart();
        }

        void displayValuesLineChart()
        {
            //Check if Values should be shown
            if (_doGraphUpdate)
            {
                for (int i = 0; i < GraphElements.LineChart.GraphPane.CurveList.Count; i++)
                {
                    var y = m_protocolHandler.m_floatValues[i];
                    GraphElements.LineChart.GraphPane.CurveList[i].AddPoint(GraphElements.x, y);    //Add new Point to the Curve
                    GraphElements.LineChart.GraphPane.CurveList[i].IsVisible = SettingsElements.m_enableButtons[i].Checked; //Apply Visibility property
                    ConfigObject.configuration.channels[i].enable = SettingsElements.m_enableButtons[i].Checked;
                }
                zedGraphLineChart.Invalidate(); //Update Graph
                zedGraphLineChart.AxisChange(); //Update Axis
                //Move Axis
                GraphElements.LineChart.GraphPane.XAxis.Scale.Max = GraphElements.x - ConfigObject.configuration.axis[0].min;
                GraphElements.LineChart.GraphPane.XAxis.Scale.Min = GraphElements.x - ConfigObject.configuration.axis[0].max;
                GraphElements.x += 1;
            }   
        }

        void displayValuesBarChart()
        {
            //Check if Values should be shown
            if (_doGraphUpdate)
            {
                for (int i = 0; i < GraphElements.BarChart.GraphPane.CurveList.Count; i++)
                {
                    GraphElements.barData[i] = m_protocolHandler.m_floatValues[i];
                    GraphElements.BarChart.GraphPane.CurveList[i][i].X = 0;
                    GraphElements.BarChart.GraphPane.CurveList[i][i].Y = (int)GraphElements.barData[i]; //Change Y Value of Curve
                    GraphElements.BarChart.GraphPane.CurveList[i].IsVisible = SettingsElements.m_enableButtons[i].Checked;
                }
                zedGraphBarChart.Invalidate();  //Update Graph
                zedGraphBarChart.AxisChange();  //Update Axis
            }
        }
        void ToggleAcq()
        {
            if (acqOnOffMenuItem.Checked)
                acqOnOffMenuItem.Checked = false;
            else
                acqOnOffMenuItem.Checked = true;
            OnAcqOnOffMenue(null, null);
        }

        public void DoPrint(string message)
        {
            m_logOutput.Items.Add(message);  //Schneide String zu wenn > MaxLength

            m_logOutput.SetSelected(m_logOutput.Items.Count - 1, true);
            if (m_logOutput.Items.Count > 255)  //Löscht 1. Zeile wenn maximale Zeilenanzahl von 255 Überschritten wurde
            {
                m_logOutput.Items.RemoveAt(0);
            }
        }

        private void saveToCSVToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_protocolHandler.SaveToCsv();
        }

        private void timerEnable()
        {
            connectToolStripMenuItem.Checked = true;
            saveToCSVToolStripMenuItem.Enabled = true;
            statusCheckbox.Checked = true;
            sendButton.Enabled = true;
            pauseResumeButton.Enabled = true;
            for (var i = 0; i < 10; i++)
            {
                m_protocolHandler.m_iValueSink[i] = _ivsBuffer[i];
            }
            _decodeTimer.Enabled = true;
        }

        private void timerDisable()
        {
            connectToolStripMenuItem.Checked = false;
            saveToCSVToolStripMenuItem.Enabled = false;
            statusCheckbox.Checked = false;
            sendButton.Enabled = false;
            pauseResumeButton.Enabled = false;
            _decodeTimer.Enabled = false;
        }

        private void sendMessage()
        {
            if(sendBox.Text.Length > 0)
            {
                m_protocolHandler.SendMessage(sendBox.Text);
            }
        }
        private void SendButton_Click(object sender, EventArgs e)
        {
            sendMessage();
        }

        private void sendBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                sendMessage();
            }
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
	        if (connectToolStripMenuItem.Checked)
	        {
		        if (ConfigCommunication())  
		        {
                    this.statusCheckbox.Enabled = true;
                    timerEnable();  //Enable Timer if connection was successful
		        }
		        else
		        {
                    this.statusCheckbox.Enabled = false;
                    timerDisable(); //Disable Timer if connection wasn't successful
                }
	        }
	        else
	        {
                timerDisable();			
		        CloseCommunication();			
	        }
        }

        private void AboutToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            try
            {
                about.Show();
                about.Focus();
            }
            catch(ObjectDisposedException)
            {
                about = new About();
                about.Show();

                about.Focus();
            }           
        }

        private void ConfigurationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var settings = new Settings();
            settings.Show();
        }

        private void EnableChannel1_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[0].enable = SettingsElements.m_enableButtons[0].Checked;
        }
        private void EnableChannel2_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[1].enable = SettingsElements.m_enableButtons[1].Checked;
        }
        private void EnableChannel3_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[2].enable = SettingsElements.m_enableButtons[2].Checked;
        }
        private void EnableChannel4_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[3].enable = SettingsElements.m_enableButtons[3].Checked;
        }
        private void EnableChannel5_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[4].enable = SettingsElements.m_enableButtons[4].Checked;
        }
        private void EnableChannel6_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[5].enable = SettingsElements.m_enableButtons[5].Checked;
        }
        private void EnableChannel7_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[6].enable = SettingsElements.m_enableButtons[6].Checked;
        }
        private void EnableChannel8_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[7].enable = SettingsElements.m_enableButtons[7].Checked;
        }
        private void EnableChannel9_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[8].enable = SettingsElements.m_enableButtons[8].Checked;
        }

        private void SetY1Ch1_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[0].IsY2 = !SettingsElements.m_selectY1[0].Checked;
        }
        private void SetY1Ch2_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[1].IsY2 = !SettingsElements.m_selectY1[1].Checked;
        }
        private void SetY1Ch3_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[2].IsY2 = !SettingsElements.m_selectY1[2].Checked;
        }
        private void SetY1Ch4_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[3].IsY2 = !SettingsElements.m_selectY1[3].Checked;
        }
        private void SetY1Ch5_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[4].IsY2 = !SettingsElements.m_selectY1[4].Checked;
        }
        private void SetY1Ch6_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[5].IsY2 = !SettingsElements.m_selectY1[5].Checked;
        }
        private void SetY1Ch7_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[6].IsY2 = !SettingsElements.m_selectY1[6].Checked;
        }
        private void SetY1Ch8_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[7].IsY2 = !SettingsElements.m_selectY1[7].Checked;
        }
        private void SetY1Ch9_CheckedChanged(object sender, EventArgs e)
        {
            ConfigObject.configuration.channels[8].IsY2 = !SettingsElements.m_selectY1[8].Checked;
        }

        private void PauseResumeButton_Click(object sender, EventArgs e)
        {
            _doGraphUpdate = !_doGraphUpdate;
            pauseResumeButton.Text = _doGraphUpdate ? "Pause" : "Resume";
        }

        private void VisForm3_Load(object sender, EventArgs e)
        {

        }

        private void lightToolStripMenuItem_Click(object sender, EventArgs e)
        {

            ThemeHandler.theme = vis1.Properties.Resources.ThemeLight;
            ThemeHandler.ApplyTheme(this);
            ThemeHandler.ApplyMenu(this.menuStrip1.Items);
            lightToolStripMenuItem.Checked = true;
            darkToolStripMenuItem.Checked = false;
        }

        private void darkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ThemeHandler.theme = vis1.Properties.Resources.ThemeDark;
            ThemeHandler.ApplyTheme(this);
            ThemeHandler.ApplyMenu(this.menuStrip1.Items);
            lightToolStripMenuItem.Checked = false;
            darkToolStripMenuItem.Checked = true;
        }
    }

    public struct channelConfig
    {
        public bool enable;
        public string name;
        public string color;
        public bool IsY2;
    };

    public struct axisConfig
    {
        public int min;
        public int max;
    };

    public class Config
    {
        public channelConfig[] channels = new channelConfig[9];
        public axisConfig[] axis = new axisConfig[3]; //axis[0]... x-Axis ; axis[1]... y1-Axis ; axis[2]... y2-Axis
        public Config()
        {
            InitializeSettings();
        }

        public void InitializeSettings()    //Load Settings from app.config into the class
        {
            for (int i = 1; i <= 9; i++)
            {
                channels[i - 1].enable = Convert.ToBoolean(ConfigurationManager.AppSettings.Get("S" + i + "Enable"));
                channels[i - 1].name = ConfigurationManager.AppSettings.Get("S" + i + "Name");
                channels[i - 1].color = ConfigurationManager.AppSettings.Get("S" + i + "Color");
                channels[i - 1].IsY2 = Convert.ToBoolean(ConfigurationManager.AppSettings.Get("S" + i + "Y2"));
            }

            axis[0].min = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Xmin"));
            axis[0].max = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Xmax"));

            axis[1].min = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Y1min"));
            axis[1].max = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Y1max"));

            axis[2].min = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Y2min"));
            axis[2].max = Convert.ToInt32(ConfigurationManager.AppSettings.Get("Y2max"));
        }

        public void SaveSettings()  //Load Settings from Elements into the class
        {
            for (int i = 0; i < 9; i++)
            {
                channels[i].enable = SettingsElements.m_enableButtonsSettings[i].Checked;
                channels[i].name = SettingsElements.m_channelNameLabelsSettings[i].Text;
                channels[i].IsY2 = SettingsElements.m_selectY2Settings[i].Checked;
            }

            if(Convert.ToInt32(SettingsElements.m_axisValuesSettings[0].Text) < Convert.ToInt32(SettingsElements.m_axisValuesSettings[1].Text))
            {
                axis[0].min = Convert.ToInt32(SettingsElements.m_axisValuesSettings[0].Text);
                axis[0].max = Convert.ToInt32(SettingsElements.m_axisValuesSettings[1].Text);
            }

            if (Convert.ToInt32(SettingsElements.m_axisValuesSettings[2].Text) < Convert.ToInt32(SettingsElements.m_axisValuesSettings[3].Text))
            {
                axis[1].min = Convert.ToInt32(SettingsElements.m_axisValuesSettings[2].Text);
                axis[1].max = Convert.ToInt32(SettingsElements.m_axisValuesSettings[3].Text);
            }

            if (Convert.ToInt32(SettingsElements.m_axisValuesSettings[4].Text) < Convert.ToInt32(SettingsElements.m_axisValuesSettings[5].Text))
            {
                axis[2].min = Convert.ToInt32(SettingsElements.m_axisValuesSettings[4].Text);
                axis[2].max = Convert.ToInt32(SettingsElements.m_axisValuesSettings[5].Text);
            }
        }

        public void SaveToFile()    //Save Elements from class to app.config
        {
            SaveSettings();

            WriteConfigToFile("Xmin", Convert.ToString(axis[0].min));
            WriteConfigToFile("Xmax", Convert.ToString(axis[0].max));

            WriteConfigToFile("Y1min", Convert.ToString(axis[1].min));
            WriteConfigToFile("Y1max", Convert.ToString(axis[1].max));

            WriteConfigToFile("Y2min", Convert.ToString(axis[2].min));
            WriteConfigToFile("Y2max", Convert.ToString(axis[2].max));

            for (int i = 1; i <= 9; i++)
            {
                WriteConfigToFile("S" + i + "Enable", Convert.ToString(channels[i - 1].enable));
                WriteConfigToFile("S" + i + "Name", Convert.ToString(channels[i - 1].name));
                WriteConfigToFile("S" + i + "Color", Convert.ToString(channels[i - 1].color));
                WriteConfigToFile("S" + i + "Y2", Convert.ToString(channels[i - 1].IsY2));
            }
        }

        public void WriteConfigToFile(string key, string value)
        {
            try
            {
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings[key] == null)
                {
                    settings.Add(key, value);
                }
                else
                {
                    settings[key].Value = value;
                }
                configFile.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);
            }
            catch (ConfigurationErrorsException)
            {
                Console.WriteLine("Error writing app settings");
            }
        }

        public void ApplySettings() //Apply Settings from class to Elements
        {
            SaveSettings();

            for (int i = 0; i < 9; i++)
            {
                SettingsElements.m_channelNameLabels[i].Text = channels[i].name;
                SettingsElements.m_enableButtons[i].Checked = channels[i].enable;
                SettingsElements.m_selectY1[i].Checked = !channels[i].IsY2;
                SettingsElements.m_selectY2[i].Checked = channels[i].IsY2;

                GraphElements.LineChart.GraphPane.CurveList[i].Label.Text = channels[i].name;
                GraphElements.BarChart.GraphPane.CurveList[i].Label.Text = channels[i].name;
            }

            ApplyAxis();
        }

        public void ApplyAxis()
        {
            GraphElements.LineChart.Invalidate();
            GraphElements.BarChart.Invalidate();
            GraphElements.LineChart.AxisChange();
            GraphElements.BarChart.AxisChange();
            
            GraphElements.LineChart.ScrollMinX = GraphElements.x - axis[0].max;
            GraphElements.LineChart.ScrollMaxX = GraphElements.x - axis[0].min;

            GraphElements.LineChart.GraphPane.YAxis.Scale.Min = axis[1].min;
            GraphElements.LineChart.GraphPane.YAxis.Scale.Max = axis[1].max;
            GraphElements.BarChart.GraphPane.YAxis.Scale.Min = axis[1].min;
            GraphElements.BarChart.GraphPane.YAxis.Scale.Max = axis[1].max;

            GraphElements.LineChart.GraphPane.Y2Axis.Scale.Min = axis[2].min;
            GraphElements.LineChart.GraphPane.Y2Axis.Scale.Max = axis[2].max;
            GraphElements.BarChart.GraphPane.Y2Axis.Scale.Min = axis[2].min;
            GraphElements.BarChart.GraphPane.Y2Axis.Scale.Max = axis[2].max;
        }

    }

    class ConfigObject
    {
        public static Config configuration = new Config();
    }

    class SettingsElements
    {
        public static Label[] m_channelNameLabels = new Label[9];
        public static CheckBox[] m_enableButtons = new CheckBox[9];
        public static RadioButton[] m_selectY1 = new RadioButton[9];
        public static RadioButton[] m_selectY2 = new RadioButton[9];

        public static TextBox[] m_channelNameLabelsSettings = new TextBox[9];
        public static CheckBox[] m_enableButtonsSettings = new CheckBox[9];
        public static RadioButton[] m_selectY1Settings = new RadioButton[9];
        public static RadioButton[] m_selectY2Settings = new RadioButton[9];
        public static TextBox[] m_axisValuesSettings = new TextBox[6];
    }

    class GraphElements
    {
        public static double x = 0;
        public static double[] barData = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        public static ZedGraphControl BarChart;
        public static ZedGraphControl LineChart;
    }
}