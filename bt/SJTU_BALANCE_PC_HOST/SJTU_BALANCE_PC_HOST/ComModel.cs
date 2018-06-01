/**
 
 * Copyright (c) 2014-2015, Wenhuix, All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.

 * Neither the name of COMDBG nor the names of its contributors may 
 * be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 */


using System;
using System.IO.Ports;
using System.Threading;

namespace SJTU_BALANCE_PC_HOST
{
    public delegate void SerialPortEventHandler(object sender, SerialPortEventArgs e);

    public class SerialPortEventArgs : EventArgs
    {
        public bool IsOpend;
        public byte[] ReceivedBytes;
    }

    public class ComModel
    {
        private readonly SerialPort _sp = new SerialPort();

        public event SerialPortEventHandler ComReceiveDataEvent;
        public event SerialPortEventHandler ComOpenEvent;
        public event SerialPortEventHandler ComCloseEvent;

        private readonly object _thisLock = new object();

        /// <summary>
        /// When serial received data, will call this method
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (_sp.BytesToRead <= 0)
            {
                return;
            }
            //Thread Safety explain in MSDN:
            // Any public static (Shared in Visual Basic) members of this type are thread safe. 
            // Any instance members are not guaranteed to be thread safe.
            // So, we need to synchronize I/O
            lock (_thisLock)
            {
                var len = _sp.BytesToRead;
                var data = new byte[len];
                try
                {
                    _sp.Read(data, 0, len);
                }
                catch (Exception)
                {
                    //catch read exception
                }

                var args = new SerialPortEventArgs {ReceivedBytes = data};
                ComReceiveDataEvent?.Invoke(this, args);
            }
        }

        /// <summary>
        /// Send bytes to device
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        public bool Send(byte[] bytes)
        {
            if (!_sp.IsOpen)
            {
                return false;
            }

            try
            {
                _sp.Write(bytes, 0, bytes.Length);
            }
            catch (Exception)
            {
                return false;   //write failed
            }
            return true;        //write successfully
        }

        /// <summary>
        /// Open Serial port
        /// </summary>
        /// <param name="portName"></param>
        /// <param name="baudRate"></param>
        public void Open(string portName, string baudRate)
        {
            if (_sp.IsOpen)
            {
                Close();
            }
            _sp.PortName = portName;
            _sp.BaudRate = Convert.ToInt32(baudRate);
            _sp.DataBits = 8;

            /**
             *  If the Handshake property is set to None the DTR and RTS pins 
             *  are then freed up for the common use of Power, the PC on which
             *  this is being typed gives +10.99 volts on the DTR pin & +10.99
             *  volts again on the RTS pin if set to true. If set to false 
             *  it gives -9.95 volts on the DTR, -9.94 volts on the RTS. 
             *  These values are between +3 to +25 and -3 to -25 volts this 
             *  give a dead zone to allow for noise immunity.
             *  http://www.codeproject.com/Articles/678025/Serial-Comms-in-Csharp-for-Beginners
             */

            //Never delete this property
            _sp.RtsEnable = true;
            _sp.DtrEnable = true;

            var args = new SerialPortEventArgs();
            try
            {
                _sp.StopBits = StopBits.One;
                _sp.Parity = Parity.None;
                _sp.Handshake = Handshake.None;
                _sp.WriteTimeout = 1000; /*Write time out*/
                _sp.Open();
                _sp.DataReceived += DataReceived;
                args.IsOpend = true;
            }
            catch (Exception)
            {
                args.IsOpend = false;
            }

            ComOpenEvent?.Invoke(this, args);

        }


        /**
         *  Take care to avoid deadlock when calling Close on the SerialPort 
         *  in response to a GUI event.
         *   An app involving the UI and the SerialPort freezes up when closing the SerialPort
         *   Deadlock can occur if Control.Invoke() is used in serial port event handlers
         * 
         *  The typical scenario we encounter is occasional deadlock in an app 
         *  that has a data received handler trying to update the GUI at the 
         *  same time the GUI thread is trying to close the SerialPort (for 
         *  example, in response to the user clicking a Close button).
         * 
         *  The reason deadlock happens is that Close() waits for events to 
         *  finish executing before it closes the port. You can address this 
         *  problem in your apps in two ways:
         * 
         *  (1)In your event handlers, replace every Control.Invoke call with 
         *  Control.BeginInvoke, which executes asynchronously and avoids 
         *  the deadlock condition. This is commonly used for deadlock avoidance 
         *  when working with GUIs.
         *  
         *  (2)Call serialPort.Close() on a separate thread. You may prefer this
         *  because this is less invasive than updating your Invoke calls.
         */
        /// <summary>
        /// Close serial port
        /// </summary>
        public void Close()
        {
            var closeThread = new Thread(CloseSpThread);
            closeThread.Start();
        }

        /// <summary>
        /// Close serial port thread
        /// </summary>
        private void CloseSpThread()
        {
            var args = new SerialPortEventArgs();
            args.IsOpend = false;
            try
            {
                _sp.Close(); //close the serial port
                _sp.DataReceived -= DataReceived;
            }
            catch (Exception)
            {
                args.IsOpend = true;
            }
            if (ComCloseEvent != null)
            {
                ComCloseEvent.Invoke(this, args);
            }

        }

    }
}
