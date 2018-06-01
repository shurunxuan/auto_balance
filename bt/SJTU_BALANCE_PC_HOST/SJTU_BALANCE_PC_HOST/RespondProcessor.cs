using System;

namespace SJTU_BALANCE_PC_HOST
{
    public delegate void RespondEventHandler(object sender, RespondEventArgs e);

    public class RespondEventArgs : EventArgs
    {
        public byte[] Bytes;
        public string Str = null;
        public int[] Ints = null;
        public float[] Floats;
    }

    public class RespondProcessor
    {
        public event RespondEventHandler CheckVersionEvent;
        public event RespondEventHandler GetAccXEvent;
        public event RespondEventHandler GetAccYEvent;
        public event RespondEventHandler GetAccZEvent;
        public event RespondEventHandler GetGyrXEvent;
        public event RespondEventHandler GetGyrYEvent;
        public event RespondEventHandler GetGyrZEvent;
        public event RespondEventHandler GetAllMpuDataEvent;

        public event RespondEventHandler GetBalanceKpEvent;
        public event RespondEventHandler GetBalanceKiEvent;
        public event RespondEventHandler GetVelocityKpEvent;
        public event RespondEventHandler GetVelocityKiEvent;
        public event RespondEventHandler GetAngleEvent;

        public void CheckVersion(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Bytes = respond;
            CheckVersionEvent?.Invoke(this, args);
        }

        public void GetAccX(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetAccXEvent?.Invoke(this, args);
        }
        public void GetAccY(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetAccYEvent?.Invoke(this, args);
        }
        public void GetAccZ(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetAccZEvent?.Invoke(this, args);
        }
        public void GetGyrX(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetGyrXEvent?.Invoke(this, args);
        }
        public void GetGyrY(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetGyrYEvent?.Invoke(this, args);
        }
        public void GetGyrZ(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetGyrZEvent?.Invoke(this, args);
        }

        public void GetAllMpuData(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[6];
            for (var i = 0; i < 6; ++i)
                args.Floats[i] = BitConverter.ToSingle(respond, i * 4);
            GetAllMpuDataEvent?.Invoke(this, args);
        }

        public void GetBalanceKp(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetBalanceKpEvent?.Invoke(this, args);
        }

        public void GetBalanceKi(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetBalanceKiEvent?.Invoke(this, args);
        }

        public void GetVelocityKp(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetVelocityKpEvent?.Invoke(this, args);
        }

        public void GetVelocityKi(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetVelocityKiEvent?.Invoke(this, args);
        }

        public void GetAngle(byte[] respond)
        {
            var args = new RespondEventArgs();
            args.Floats = new float[1];
            args.Floats[0] = BitConverter.ToSingle(respond, 0);
            GetAngleEvent?.Invoke(this, args);
        }

    }
}
