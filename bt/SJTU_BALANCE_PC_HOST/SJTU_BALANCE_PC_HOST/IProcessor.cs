using System;
using System.Collections;

namespace SJTU_BALANCE_PC_HOST
{
    public interface IProcessView
    {
        void SetProcessor(Processor processor);
        void CheckVersionEvent(object sender, RespondEventArgs e);
        void GetAccXEvent(object sender, RespondEventArgs e);
        void GetAccYEvent(object sender, RespondEventArgs e);
        void GetAccZEvent(object sender, RespondEventArgs e);
        void GetGyrXEvent(object sender, RespondEventArgs e);
        void GetGyrYEvent(object sender, RespondEventArgs e);
        void GetGyrZEvent(object sender, RespondEventArgs e);
        void GetAllMpuDataEvent(object sender, RespondEventArgs e);

        void GetBalanceKpEvent(object sender, RespondEventArgs e);
        void GetBalanceKiEvent(object sender, RespondEventArgs e);
        void GetVelocityKpEvent(object sender, RespondEventArgs e);
        void GetVelocityKiEvent(object sender, RespondEventArgs e);
        void GetAngleEvent(object sender, RespondEventArgs e);
    }

    public class Processor
    {
        readonly RespondProcessor _respondProcessor = new RespondProcessor();
        readonly IProcessView _processView;

        private readonly ArrayList _fullRespondBytes;

        public Processor(IProcessView processView)
        {
            _processView = processView;
            _fullRespondBytes = new ArrayList();
            processView.SetProcessor(this);
            _respondProcessor.CheckVersionEvent += _processView.CheckVersionEvent;
            _respondProcessor.GetAccXEvent += _processView.GetAccXEvent;
            _respondProcessor.GetAccYEvent += _processView.GetAccYEvent;
            _respondProcessor.GetAccZEvent += _processView.GetAccZEvent;
            _respondProcessor.GetGyrXEvent += _processView.GetGyrXEvent;
            _respondProcessor.GetGyrYEvent += _processView.GetGyrYEvent;
            _respondProcessor.GetGyrZEvent += _processView.GetGyrZEvent;
            _respondProcessor.GetAllMpuDataEvent += _processView.GetAllMpuDataEvent;

            _respondProcessor.GetBalanceKpEvent += _processView.GetBalanceKpEvent;
            _respondProcessor.GetBalanceKiEvent += _processView.GetBalanceKiEvent;
            _respondProcessor.GetVelocityKpEvent += _processView.GetVelocityKpEvent;
            _respondProcessor.GetVelocityKiEvent += _processView.GetVelocityKiEvent;
            _respondProcessor.GetAngleEvent += _processView.GetAngleEvent;
        }

        public void ProcessResopnd(byte[] commandBytes, int expectedLength, byte[] receivedBytes)
        {
            _fullRespondBytes.AddRange(receivedBytes);
            // Received enough bytes
            if (_fullRespondBytes.Count < expectedLength) return;
            // Extract respond
            var resopndBytes = new byte[expectedLength];
            for (var i = 0; i < expectedLength; ++i)
                resopndBytes[i] = (byte)_fullRespondBytes[i];
            // Clear ArrayList
            _fullRespondBytes.Clear();


            // Check Version
            if (commandBytes[0] == 0)
            {
                _respondProcessor.CheckVersion(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 1)
            {
                _respondProcessor.GetAccX(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 2)
            {
                _respondProcessor.GetAccY(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 3)
            {
                _respondProcessor.GetAccZ(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 4)
            {
                _respondProcessor.GetGyrX(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 5)
            {
                _respondProcessor.GetGyrY(resopndBytes);
            }
            // Get ACC_X
            else if (commandBytes[0] == 6)
            {
                _respondProcessor.GetGyrZ(resopndBytes);
            }
            // Get All Data
            else if (commandBytes[0] == 7)
            {
                _respondProcessor.GetAllMpuData(resopndBytes);
            }
            // Get Balance K_p
            else if (commandBytes[0] == 0x11)
            {
                _respondProcessor.GetBalanceKp(resopndBytes);
            }
            // Get Balance K_i
            else if (commandBytes[0] == 0x12)
            {
                _respondProcessor.GetBalanceKi(resopndBytes);
            }
            // Get Velocity K_p
            else if (commandBytes[0] == 0x13)
            {
                _respondProcessor.GetVelocityKp(resopndBytes);
            }
            // Get Velocity K_i
            else if (commandBytes[0] == 0x14)
            {
                _respondProcessor.GetVelocityKi(resopndBytes);
            }
            // Get Angle
            else if (commandBytes[0] == 0x31)
            {
                _respondProcessor.GetAngle(resopndBytes);
            }

        }
    }
}
