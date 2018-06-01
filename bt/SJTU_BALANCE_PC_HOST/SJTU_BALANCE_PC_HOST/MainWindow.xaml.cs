using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO.Ports;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

//using System.Threading.Tasks;
//using System.Runtime.InteropServices.WindowsRuntime;
//using Windows.Devices.Enumeration;
//using Windows.Devices.Bluetooth.GenericAttributeProfile;

namespace SJTU_BALANCE_PC_HOST
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, IComView, IProcessView
    {
        private Controller _controller;
        private Processor _processor;

        private byte[] _commandBytes;
        private int _expectedLength;
        private byte[] _receivedBytes;

        private DataGraph _graph;
        private readonly bool[] _showLine;

        Timer _mpuTimer;



        public struct MpuData
        {
            public float AccX;
            public float AccY;
            public float AccZ;
            public float GyrX;
            public float GyrY;
            public float GyrZ;
        }

        private MpuData _mpuData;


        /// <summary>
        /// Set controller
        /// </summary>
        /// <param name="controller"></param>
        public void SetController(Controller controller)
        {
            _controller = controller;
        }

        public void SetProcessor(Processor processor)
        {
            _processor = processor;
        }

        /// <summary>
        /// update status bar
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void OpenComEvent(object sender, SerialPortEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.Invoke(DispatcherPriority.Send, new Action<object, SerialPortEventArgs>(OpenComEvent), sender, e);
                return;
            }

            if (e.IsOpend)  //Open successfully
            {
                OpenPortButton.Content = "Close Port";
                //sendbtn.Enabled = true;
                //autoSendcbx.Enabled = true;
                //autoReplyCbx.Enabled = true;

                PortNameCombo.IsEnabled = false;
                BaudRateCombo.IsEnabled = false;
            }
            else    //Open failed
            {
                MessageBox.Show("Open Port Failed!", "ERROR");
                //sendbtn.Enabled = false;
                //autoSendcbx.Enabled = false;
                //autoReplyCbx.Enabled = false;
            }
        }

        /// <summary>
        /// update status bar
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void CloseComEvent(object sender, SerialPortEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.Invoke(DispatcherPriority.Send, new Action<object, SerialPortEventArgs>(CloseComEvent), sender, e);
                return;
            }

            if (!e.IsOpend) //close successfully
            {
                OpenPortButton.Content = "Open Port";

                //sendbtn.Enabled = false;
                //sendtbx.ReadOnly = false;
                //autoSendcbx.Enabled = false;
                //autoSendtimer.Stop();

                PortNameCombo.IsEnabled = true;
                BaudRateCombo.IsEnabled = true;
            }
        }

        /// <summary>
        /// Display received data
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void ComReceiveDataEvent(object sender, SerialPortEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                try
                {
                    Dispatcher.Invoke(DispatcherPriority.Send, new Action<object, SerialPortEventArgs>(ComReceiveDataEvent), sender, e);
                }
                catch (Exception)
                {
                    //disable form destroy exception
                }
                return;
            }

            _receivedBytes = e.ReceivedBytes;

            _processor.ProcessResopnd(_commandBytes, _expectedLength, _receivedBytes);


            //if (recStrRadiobtn.Checked) //display as string
            //{
            //    receivetbx.AppendText(Encoding.Default.GetString(e.receivedBytes));
            //}
            //else //display as hex
            //{
            //    if (receivetbx.Text.Length > 0)
            //    {
            //        receivetbx.AppendText("-");
            //    }
            //    receivetbx.AppendText(IController.Bytes2Hex(e.receivedBytes));
            //}

        }

        public void CheckVersionEvent(object sender, RespondEventArgs e)
        {
            string[] str = { "Current Version is ", e.Bytes[0].ToString(), ".", e.Bytes[1].ToString(), "!" };

            MessageBox.Show(string.Concat(str), "Version Check");
        }

        public void GetAccXEvent(object sender, RespondEventArgs e)
        {
            _mpuData.AccX = e.Floats[0];
            RefreshMpuStatusBox();
        }
        public void GetAccYEvent(object sender, RespondEventArgs e)
        {
            _mpuData.AccY = e.Floats[0];
            RefreshMpuStatusBox();
        }
        public void GetAccZEvent(object sender, RespondEventArgs e)
        {
            _mpuData.AccZ = e.Floats[0];
            RefreshMpuStatusBox();
        }
        public void GetGyrXEvent(object sender, RespondEventArgs e)
        {
            _mpuData.GyrX = e.Floats[0];
            RefreshMpuStatusBox();
        }
        public void GetGyrYEvent(object sender, RespondEventArgs e)
        {
            _mpuData.GyrY = e.Floats[0];
            RefreshMpuStatusBox();
        }
        public void GetGyrZEvent(object sender, RespondEventArgs e)
        {
            _mpuData.GyrZ = e.Floats[0];
            RefreshMpuStatusBox();
        }

        public void GetAllMpuDataEvent(object sender, RespondEventArgs e)
        {
            _mpuData.AccX = e.Floats[0];
            _mpuData.AccY = e.Floats[1];
            _mpuData.AccZ = e.Floats[2];
            _mpuData.GyrX = e.Floats[3];
            _mpuData.GyrY = e.Floats[4];
            _mpuData.GyrZ = e.Floats[5];
            RefreshMpuStatusBox();
        }

        public void GetBalanceKpEvent(object sender, RespondEventArgs e)
        {
            BalanceKpTextBox.Text = e.Floats[0].ToString(CultureInfo.InvariantCulture);
        }

        public void GetBalanceKiEvent(object sender, RespondEventArgs e)
        {
            BalanceKiTextBox.Text = e.Floats[0].ToString(CultureInfo.InvariantCulture);
        }

        public void GetVelocityKpEvent(object sender, RespondEventArgs e)
        {
            VelocityKpTextBox.Text = e.Floats[0].ToString(CultureInfo.InvariantCulture);
        }

        public void GetVelocityKiEvent(object sender, RespondEventArgs e)
        {
            VelocityKiTextBox.Text = e.Floats[0].ToString(CultureInfo.InvariantCulture);
        }

        public void GetAngleEvent(object sender, RespondEventArgs e)
        {
            AngleTextBox.Text = e.Floats[0].ToString(CultureInfo.InvariantCulture);
        }


        public MainWindow()
        {
            _commandBytes = null;
            _expectedLength = 0;
            _receivedBytes = null;
            _showLine = new[] { true, true, true, true, true, true };

            InitializeComponent();
            Initialize();
        }

        private void Initialize()
        {
            //Com Ports
            var arrayComPortsNames = SerialPort.GetPortNames();
            if (arrayComPortsNames.Length == 0)
            {
                OpenPortButton.IsEnabled = false;
            }
            else
            {
                Array.Sort(arrayComPortsNames);

                var listString = new List<string>();
                foreach (var eachString in arrayComPortsNames)
                {
                    if (!listString.Contains(eachString))
                        listString.Add(eachString);
                }

                foreach (var name in listString)
                {
                    PortNameCombo.Items.Add(name);
                }
                PortNameCombo.SelectedIndex = listString.Count - 1;
                OpenPortButton.IsEnabled = true;
            }


            // Timer
            _mpuTimer = new Timer();
            _mpuTimer.Stop();
            _mpuTimer.Interval = 100;
            _mpuTimer.Elapsed += MpuDataMpuTimerElapsed;


            // Data Graph
            _graph = new DataGraph(MpuCanvas, Model, _mpuTimer);

            // Legend
            LegendCheckBox0.Foreground = new SolidColorBrush(_graph.Colors[0]);
            LegendCheckBox1.Foreground = new SolidColorBrush(_graph.Colors[1]);
            LegendCheckBox2.Foreground = new SolidColorBrush(_graph.Colors[2]);
            LegendCheckBox3.Foreground = new SolidColorBrush(_graph.Colors[3]);
            LegendCheckBox4.Foreground = new SolidColorBrush(_graph.Colors[4]);
            LegendCheckBox5.Foreground = new SolidColorBrush(_graph.Colors[5]);




        }

        void MpuDataMpuTimerElapsed(object sender, ElapsedEventArgs e)
        {
            _commandBytes = new[] { (byte)7, (byte)'$' };
            _expectedLength = 24;

            var success = _controller.SendDataToCom(_commandBytes);
        }


        private void RefreshMpuStatusBox()
        {
            AccXLabel.Content = _mpuData.AccX.ToString(CultureInfo.InvariantCulture);
            AccYLabel.Content = _mpuData.AccY.ToString(CultureInfo.InvariantCulture);
            AccZLabel.Content = _mpuData.AccZ.ToString(CultureInfo.InvariantCulture);
            GyrXLabel.Content = _mpuData.GyrX.ToString(CultureInfo.InvariantCulture);
            GyrYLabel.Content = _mpuData.GyrY.ToString(CultureInfo.InvariantCulture);
            GyrZLabel.Content = _mpuData.GyrZ.ToString(CultureInfo.InvariantCulture);
            _graph.SetShow(_showLine);
            _graph.AddData(_mpuData);
            _graph.Update();
        }

        private void OpenPortButton_Click(object sender, RoutedEventArgs e)
        {
            if (OpenPortButton != null && (string)OpenPortButton.Content == "Open Port")
            {
                _controller.OpenSerialPort(PortNameCombo.Text, BaudRateCombo.Text);
            }
            else
            {
                _controller.CloseSerialPort();
            }
        }

        private void CheckVersionButton_Click(object sender, RoutedEventArgs e)
        {

            _commandBytes = new[] { (byte)0, (byte)'$' };
            _expectedLength = 2;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetAccXButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)1, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }
        private void GetAccYButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)2, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }
        private void GetAccZButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)3, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }
        private void GetGyrXButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)4, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }
        private void GetGyrYButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)5, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }
        private void GetGyrZButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)6, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetAllMpuDataButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)7, (byte)'$' };
            _expectedLength = 24;

            var success = _controller.SendDataToCom(_commandBytes);
        }



        private void GetBalanceKpButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)0x11, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetBalanceKiButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)0x12, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetVelocityKpButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)0x13, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetVelocityKiButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)0x14, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void GetAngleButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new[] { (byte)0x31, (byte)'$' };
            _expectedLength = 4;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetBalanceKpButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x15;
            _commandBytes[5] = (byte)'$';
            
            var requestBytes = BitConverter.GetBytes(float.Parse(BalanceKpTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte) '$')
                {
                    var parsed = float.Parse(BalanceKpTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetBalanceKiButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x16;
            _commandBytes[5] = (byte)'$';

            var requestBytes = BitConverter.GetBytes(float.Parse(BalanceKiTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte)'$')
                {
                    var parsed = float.Parse(BalanceKiTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetVelocityKpButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x17;
            _commandBytes[5] = (byte)'$';

            var requestBytes = BitConverter.GetBytes(float.Parse(VelocityKpTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte)'$')
                {
                    var parsed = float.Parse(VelocityKpTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetVelocityKiButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x18;
            _commandBytes[5] = (byte)'$';

            var requestBytes = BitConverter.GetBytes(float.Parse(VelocityKiTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte)'$')
                {
                    var parsed = float.Parse(VelocityKiTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetAngleButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x32;
            _commandBytes[5] = (byte)'$';

            var requestBytes = BitConverter.GetBytes(float.Parse(AngleTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte)'$')
                {
                    var parsed = float.Parse(AngleTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void SetSpeedButton_Click(object sender, RoutedEventArgs e)
        {
            _commandBytes = new byte[6];
            _commandBytes[0] = 0x20;
            _commandBytes[5] = (byte)'$';

            var requestBytes = BitConverter.GetBytes(float.Parse(SpeedTextBox.Text));
            for (int i = 0; i < requestBytes.Length; ++i)
            {
                if (requestBytes[i] == (byte)'$')
                {
                    var parsed = float.Parse(SpeedTextBox.Text);
                    var getbytes = BitConverter.GetBytes(parsed);
                    var bytesstr = BitConverter.ToString(getbytes);
                    var restored = BitConverter.ToSingle(getbytes, 0);
                    MessageBox.Show(String.Concat("Can't set this value!\n", parsed, '\n', bytesstr, '\n', restored));
                    return;
                }
                _commandBytes[i + 1] = requestBytes[i];
            }
            _expectedLength = 0;

            var success = _controller.SendDataToCom(_commandBytes);
        }

        private void AutoUpdateCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            _mpuTimer.Start();
        }

        private void AutoUpdateCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            _mpuTimer.Stop();
        }

        private void LegendCheckBox0_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[0] = true;
        }

        private void LegendCheckBox0_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[0] = false;
        }

        private void LegendCheckBox1_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[1] = true;
        }

        private void LegendCheckBox1_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[1] = false;
        }

        private void LegendCheckBox2_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[2] = true;
        }

        private void LegendCheckBox2_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[2] = false;
        }

        private void LegendCheckBox3_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[3] = true;
        }

        private void LegendCheckBox3_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[3] = false;
        }

        private void LegendCheckBox4_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[4] = true;
        }

        private void LegendCheckBox4_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[4] = false;
        }

        private void LegendCheckBox5_Checked(object sender, RoutedEventArgs e)
        {
            _showLine[5] = true;
        }

        private void LegendCheckBox5_Unchecked(object sender, RoutedEventArgs e)
        {
            _showLine[5] = false;
        }

        private void PostureResetButton_Click(object sender, RoutedEventArgs e)
        {
            _graph.ResetCalculator();
        }

        private void MainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            _graph.Resize();
        }

    }
}
