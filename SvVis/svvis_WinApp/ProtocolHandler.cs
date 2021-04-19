using System;
using System.IO;
using ZedHL;
using System.Windows.Forms;
using System.IO.Ports;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;


namespace vis1
{
    public enum Scaling
    {
        None = 0,
        Q15 = 1,
    }

    public interface IPrintCB
    {
        void DoPrint(string message);
    }

    class ProtocolHandler
    {
        #region Member Variables
        protected SerialPort m_serialPort;
        protected TcpClient m_socket;
        protected int m_device;
        protected BinaryReaderEx m_binaryReader;
        protected Stopwatch m_stopWatch = new Stopwatch();
        protected int m_valSum;
        protected IPrintCB m_printCallback;

        protected const int device_none = 0;
        protected const int device_com = 1;
        protected const int device_socket = 2;

        public Scaling m_scale = Scaling.None;
        public BinaryWriterEx m_binaryWriter;
        public short[] m_shortValues = new short[10];
        public float[] m_floatValues = new float[10];
        public IValueSink[] m_iValueSink = new IValueSink[10];
        public ByteRingBuffer[] m_ringBuffer = new ByteRingBuffer[10];
        public int NVals, NBytes;
        public double valsPerSec;
        #endregion

        public ProtocolHandler(SerialPort aPort, IPrintCB aPrintObj)
        {
            m_device = device_com;
            m_serialPort = aPort;
            m_binaryReader = new BinaryReaderEx(m_serialPort.BaseStream); //Liest von aPort
            m_binaryWriter = new BinaryWriterEx(m_serialPort.BaseStream);   //Schreibt auf aPort
            m_printCallback = aPrintObj;
            for (var i = 0; i < m_iValueSink.Length; i++)
                m_iValueSink[i] = new DummyValueSink();
            m_stopWatch.Reset(); m_stopWatch.Start(); //Stopuhr neustarten

            NVals = 9; NBytes = 3 * NVals;
        }

        public ProtocolHandler(TcpClient aSock, IPrintCB aPrintObj)
        {
            m_device = device_socket;
            m_socket = aSock;
            m_binaryReader = new BinaryReaderEx(m_socket.GetStream()); // Liest von Socket
            m_binaryWriter = new BinaryWriterEx(m_socket.GetStream()); // Schreibt auf socket
            m_printCallback = aPrintObj;
            for (var i = 0; i < m_iValueSink.Length; i++)
                m_iValueSink[i] = new DummyValueSink();
            m_stopWatch.Reset(); m_stopWatch.Start();

            NVals = 9; NBytes = 3 * NVals;
        }

        //Counter for read errors
        public int Readerrorcnt = 0;

        //Scaling factor
        private const float C1 = (float)1.0 / short.MaxValue;

        //Queues for exporting to File
        private readonly Queue<float> _logfloat = new Queue<float>();
        private readonly Queue<int> _logchannel = new Queue<int>();

        //Maximum size for queues TODO: sollte x-Axe entsprechen
        private const int QueuMaxSize = 20 * 100;

        //Add adae to queues and chart
        private void AddData(int ID, float data)
        {
            //chart
            m_floatValues[ID] = data;
            //m_iValueSink[ID].AddValue(m_floatValues[ID]);

            _logfloat.Enqueue(data); //Daten in Query speichern
            _logchannel.Enqueue(ID); //Kanalnummer in Query Speichern

            //Delete one item if queue maximum size is reched
            if (_logchannel.Count > QueuMaxSize)
            {
                _logfloat.Dequeue();
                _logchannel.Dequeue();
            }
        }


        public bool CheckValsPerSecond()    //Z‰hlt werte pro Sekunde
        {
            if (m_stopWatch.ElapsedMilliseconds > 1000) //nach einer Sekunde
            {
                m_stopWatch.Stop();     //Stopuhr anhalten
                valsPerSec = m_valSum / (m_stopWatch.ElapsedMilliseconds / 1000.0); //Berechnen der Werte pro Sekunde
                m_valSum = 0; m_stopWatch.Reset(); m_stopWatch.Start(); //Wertez‰hler und Stopuhr zur¸cksetzen
                return true;
            }
            return false;
        }

        public void Flush()
        {
            m_binaryWriter.Flush();
        }

        public void WriteSv16(byte aId, short aVal)
        {
            m_binaryWriter.WriteSv16(aId, aVal);   //Sende ID + 2Byte Daten
        }

        public void SendMessage(string message)
        {
            byte[] msg = Encoding.ASCII.GetBytes(message);
            byte[] buf = new byte[msg.Length +2];
            msg.CopyTo(buf,1);

            // type code 10
            buf[0] = 10;
            // null terminator
            buf[buf.Length-1] = 0;
            m_binaryWriter.Write(buf, 0, buf.Length);
        }

        public virtual void AddData()
        {
        }

        // detect if a socket is still connected to its peer
        public static bool SocketIsConnected(Socket socket)
        {
            // copy-paste from stackoverflow
            try
            {
                return !(socket.Poll(1, SelectMode.SelectRead) && socket.Available == 0);
            }
            catch(SocketException) { return false; }
        }

        // check if a COM Port is still opened
        public static bool SerialPortIsOpen(SerialPort port)
        {
            return port.IsOpen;
        }

        // parses all ProtocolPacket's with all Variables
        public bool ParseAllPackets()
        {
            try
            {
                // COM - port und sockets verwenden verschiedene methoden um die Anzahl
                // der noch nicht verarbeiteten Bytes zu ermitteln
                if (m_device == device_com)
                {
                    if(!SerialPortIsOpen(m_serialPort))
                    {
                        // serial port closed, throw an exception to close the communication
                        throw new InvalidOperationException();
                    }
                    if (m_serialPort.BytesToRead < 3) //Mindestens 3 Byte (ID + 2 Byte Daten)
                    {
                        return false;
                    }
                }
                else if(m_device == device_socket)
                {
                    if(!SocketIsConnected(m_socket.Client))
                    {
                        // socket disconnected, throw an exception to close the communication
                        throw new InvalidOperationException();
                    }
                    if(m_socket.Available < 3)
                    {
                        return false;
                    }
                }
                else
                {
                    // Invalid code execution happened, leading to m_device being set to an illegal value
                    throw new InvalidOperationException();
                }
            }
            catch (IOException)
            {
                MessageBox.Show("Disconnected!");

                return false;
            }

            while (  ((m_device == device_com)&&(m_serialPort.BytesToRead >= 3)) || ((m_device == device_socket)&&(m_socket.Available >= 3))  )
            {
                int id = m_binaryReader.ReadByte() - 1; //Liest erstes Byte (aID) -> wird vewendet um Datentyp zuzuordnen

                //CHANGE: continue mit else if ersetzt!
                if (id == 9) //ID 9: string SV
                {
                    string test = m_binaryReader.ReadCString();
                    m_printCallback.DoPrint(test);
                    //continue;
                }
                else if (id >= 0 && id <= 8)  //ID 0 bis 8: 3.13 Format (2 Byte)
                {
                    AddData(id, m_binaryReader.Read3P13());
                    //continue;
                }
                else if (id >= 10 && id <= 19)    //ID 10 bis 19: short (2 Byte)
                {
                    if (m_scale == Scaling.Q15) //_scal == 1
                        AddData(id - 10, C1 * m_binaryReader.ReadInt16());
                    else
                        AddData(id - 10, m_binaryReader.ReadInt16());
                    //continue;
                }
                else if (id >= 20 && id <= 29)    //ID 20 bis 29: float
                {
                    AddData(id - 20, m_binaryReader.ReadSingle());
                    //continue;
                }
            }
            return true;
        }

        public void SaveToCsv()
        {
            //StreamWriter w writes on filePath
            var saveFileDialog1 = new SaveFileDialog();

            //FILENAME: Logs_<DATE>_<TIME>.csv
            var localDate = DateTime.Now;
            var culture = new CultureInfo("de-DE");
            var date = localDate.ToString(culture).Replace(".", "_").Replace(" ", "_").Replace(":", "_");
            saveFileDialog1.FileName = "Log_" + date + ".csv";

            saveFileDialog1.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Stream csvStream;

                if ((csvStream = saveFileDialog1.OpenFile()) != null)
                {
                    using (var w = new StreamWriter(csvStream))
                    {
                        var line = "n;";    //Spaltenbeschriftung
                        for (int i = 0; i < 9; i++)
                        {
                            line = line + ConfigObject.configuration.channels[i].name + ";";
                        }
                        w.WriteLine(line);
                        w.Flush();

                        bool[] set = { false, false, false, false, false, false, false, false, false, false };
                        string[] value = { "", "", "", "", "", "", "", "", "", "" };

                        //While data in Query
                        var n = 1;
                        while (_logchannel.Count > 0 && _logfloat.Count > 0)
                        {
                            var channel = _logchannel.Dequeue(); //Get Channel-Number

                            if (set[channel])   //Write Line if value is already in Buffer
                            {
                                line = $"{n};{value[0]};{value[1]};{value[2]};{value[3]};{value[4]};{value[5]};{value[6]};{value[7]};{value[8]};{value[9]};";
                                w.WriteLine(line);
                                w.Flush();
                                n++;

                                for (var i = 0; i < 10; i++)    //Reset Values
                                {
                                    set[i] = false;
                                    value[i] = "";
                                }
                            }
                            value[channel] = _logfloat.Dequeue().ToString().Replace(".", ",");  //Auf Europ‰isches Format umwandeln . -> ,
                            set[channel] = true;
                        }
                        //Restliche Werte auch Schreiben
                        line = $"{n};{value[0]};{value[1]};{value[2]};{value[3]};{value[4]};{value[5]};{value[6]};{value[7]};{value[8]};{value[9]};";
                        w.WriteLine(line);
                        w.Flush();
                    }
                    csvStream.Close();
                }
            }
            //Query Leeren
            _logfloat.Clear();
            _logchannel.Clear();
        }

        public virtual void SwitchAcq(bool aOnOff)
        {
            if (aOnOff)
            {
                m_binaryWriter.WriteSv16(10, 1);
                // binWr.Write((byte)1);
            }
            else
            {
                m_binaryWriter.WriteSv16(10, 0);
                // binWr.Write((byte)0);
            }
        }

        public virtual void Close() //Stream schlieﬂen
        {
            try
            {
                m_binaryWriter.Close();  //Binary Writer schlieﬂen
                m_binaryReader.Close();  //Binary Reader schlieﬂen
            }
            catch (IOException)
            {

            }
        }
    }
}