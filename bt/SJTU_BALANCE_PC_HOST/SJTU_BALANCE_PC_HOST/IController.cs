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
using System.Text;

namespace SJTU_BALANCE_PC_HOST
{
    public interface IComView
    {
        void SetController(Controller controller);
        //Open serial port event
        void OpenComEvent(object sender, SerialPortEventArgs e);
        //Close serial port event
        void CloseComEvent(object sender, SerialPortEventArgs e);
        //Serial port receive data event
        void ComReceiveDataEvent(object sender, SerialPortEventArgs e);
    }

    public class Controller
    {
        readonly ComModel _comModel = new ComModel();
        IComView _comView;

        public Controller(IComView comView)
        {
            _comView = comView;
            comView.SetController(this);
            _comModel.ComCloseEvent += comView.CloseComEvent;
            _comModel.ComOpenEvent += comView.OpenComEvent;
            _comModel.ComReceiveDataEvent += comView.ComReceiveDataEvent;
        }

        /// <summary>
        /// Hex to byte
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        private static byte[] FromHex(string hex)
        {
            hex = hex.Replace("-", "");
            var raw = new byte[hex.Length / 2];
            for (var i = 0; i < raw.Length; i++)
            {
                try
                {
                    raw[i] = Convert.ToByte(hex.Substring(i * 2, 2), 16);
                }
                catch (Exception)
                {
                    //Do Nothing
                }

            }
            return raw;
        }

        /// <summary>
        /// Hex string to string
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        public static string Hex2String(string hex)
        {
            var data = FromHex(hex);
            return Encoding.Default.GetString(data);
        }

        /// <summary>
        /// String to hex string
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string String2Hex(string str)
        {
            var data = Encoding.Default.GetBytes(str);
            return BitConverter.ToString(data);
        }

        /// <summary>
        /// Hex string to bytes
        /// </summary>
        /// <param name="hex"></param>
        /// <returns></returns>
        public static byte[] Hex2Bytes(string hex)
        {
            return FromHex(hex);
        }

        /// <summary>
        /// Bytes to Hex String
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        public static string Bytes2Hex(byte[] bytes)
        {
            return BitConverter.ToString(bytes);
        }

        /// <summary>
        /// send bytes to serial port
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public bool SendDataToCom(byte[] data)
        {
            return _comModel.Send(data);
        }

        /// <summary>
        /// Send string to serial port
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public bool SendDataToCom(string str)
        {
            if (!string.IsNullOrEmpty(str))
            {
                return _comModel.Send(Encoding.Default.GetBytes(str));
            }
            return true;
        }

        /// <summary>
        /// Open serial port in comModel
        /// </summary>
        /// <param name="portName"></param>
        /// <param name="baudRate"></param>
        public void OpenSerialPort(string portName, string baudRate)
        {
            if (!string.IsNullOrEmpty(portName))
            {
                _comModel.Open(portName, baudRate);
            }
        }

        /// <summary>
        /// Close serial port in comModel
        /// </summary>
        public void CloseSerialPort()
        {
            _comModel.Close();
        }

    }
}
