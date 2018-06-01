using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;
using System.Globalization;
using MathNet.Numerics.LinearAlgebra.Double;
using System.Diagnostics;
using MathNet.Numerics.LinearAlgebra;

namespace SJTU_BALANCE_PC_HOST
{
    class PostureCalculator
    {
        private const float Kp = 10.0f;  
        private const float Ki = 0.008f;            
        private readonly float _halfT;

        private double _timerInterval;

        private Vector3 _intError;

        public Quaternion Q;

        public PostureCalculator(double interval)
        {
            _timerInterval = interval;
            _halfT = (float)(interval / 125000);
            Reset();
        }

        public void Reset()
        {
            _intError = Vector3.Zero;
            Q = Quaternion.Identity;
        }

        public Vector3 UpdateData(MainWindow.MpuData mpuData)
        {
            var a = new Vector3(mpuData.AccX, mpuData.AccY, mpuData.AccZ);
            var g = new Vector3(mpuData.GyrX, mpuData.GyrY, mpuData.GyrZ);
            Vector3 v;
            Vector3 e;

            var q0 = Q.X;
            var q1 = Q.Y;
            var q2 = Q.Z;
            var q3 = Q.W;

            var q0Q0 = q0 * q0;
            var q0Q1 = q0 * q1;
            var q0Q2 = q0 * q2;
            var q0Q3 = q0 * q3;
            var q1Q1 = q1 * q1;
            var q1Q2 = q1 * q2;
            var q1Q3 = q1 * q3;
            var q2Q2 = q2 * q2;
            var q2Q3 = q2 * q3;
            var q3Q3 = q3 * q3;


            a = Vector3.Normalize(a);
            v = new Vector3(2 * (q1Q3 - q0Q2), 2 * (q0Q1 + q2Q3), q0Q0 - q1Q1 - q2Q2 + q3Q3);
            e = new Vector3(a.Y * v.Z - a.Z * v.Y, a.Z * v.X - a.X * v.Z, a.X * v.Y - a.Y * v.X);
            _intError += e * Ki;
            g += Kp * e + _intError;

            q0 = q0 + (-q1 * g.X - q2 * g.Y - q3 * g.Z) * _halfT;
            q1 = q1 + (q0 * g.X + q2 * g.Z - q3 * g.Y) * _halfT;
            q2 = q2 + (q0 * g.Y - q1 * g.Z + q3 * g.X) * _halfT;
            q3 = q3 + (q0 * g.Z + q1 * g.Y - q2 * g.X) * _halfT;

            Q.X = q0;
            Q.Y = q1;
            Q.Z = q2;
            Q.W = q3;

            Q = Quaternion.Normalize(Q);

            var eax = Math.Atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.296;
            var eay = Math.Asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.296;
            var eaz = Math.Atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.296;

            var eularAngle = new Vector3((float)eax, (float)eay, (float)eaz);

            return eularAngle;
        }

    }
}
