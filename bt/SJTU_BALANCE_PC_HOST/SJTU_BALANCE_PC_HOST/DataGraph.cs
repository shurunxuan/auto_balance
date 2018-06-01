using System;
using System.Collections;
using System.Numerics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using System.Windows.Shapes;

namespace SJTU_BALANCE_PC_HOST
{
    internal class DataGraph
    {
        private bool _canvasInitialized;
        private readonly Canvas _canvas;
        private readonly GeometryModel3D _model;

        private double _width;
        private double _height;

        private double _xMin;
        private double _xMax;
        private double _yMin;
        private double _yMax;

        private double _bound;

        private const int DataLength = 100;

        private bool[] _show;

        private readonly ArrayList[] _data;
        private readonly Polyline[] _lines;

        public Color[] Colors;

        private readonly PostureCalculator _calculator;

        private Vector3 _eularAngle;

        public DataGraph(Canvas c, GeometryModel3D m, System.Timers.Timer t)
        {
            _canvasInitialized = false;
            _canvas = c;
            _model = m;
            _data = new ArrayList[6];
            _lines = new Polyline[6];
            Colors = new Color[6];
            for (var i = 0; i < 6; ++i)
            {
                _data[i] = new ArrayList();
                _lines[i] = new Polyline();
            }

            Colors[0] = Color.FromRgb(68, 114, 196);
            Colors[1] = Color.FromRgb(237, 125, 49);
            Colors[2] = Color.FromRgb(165, 165, 165);
            Colors[3] = Color.FromRgb(255, 192, 0);
            Colors[4] = Color.FromRgb(91, 155, 213);
            Colors[5] = Color.FromRgb(112, 173, 71);

            _calculator = new PostureCalculator(t.Interval);
            _eularAngle = Vector3.Zero;

            var rotateZ = new RotateTransform3D
            {
                Rotation = new AxisAngleRotation3D(new Vector3D(0, 1, 0), 180)
            };
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(rotateZ);
            _model.Transform = transformGroup;

            for (var i = 0; i < 6; ++i)
            {
                _lines[i].Stroke = new SolidColorBrush(Colors[i]);
                _lines[i].StrokeThickness = 2;
            }

        }

        private void InitializeCanvas()
        {
            _width = _canvas.ActualWidth;
            _height = _canvas.ActualHeight;
            var xAxis = new Line();//x轴
            var yAxis = new Line();//y轴
            xAxis.Stroke = Brushes.Black;
            yAxis.Stroke = Brushes.Black;
            xAxis.StrokeThickness = 3;
            yAxis.StrokeThickness = 3;
            xAxis.X1 = 40;
            xAxis.Y1 = _height / 2.0;
            xAxis.X2 = _width - xAxis.X1;
            xAxis.Y2 = xAxis.Y1;
            yAxis.X1 = xAxis.X1;
            yAxis.Y1 = 30;
            yAxis.X2 = xAxis.X1;
            yAxis.Y2 = _height - yAxis.Y1;
            _canvas.Children.Add(xAxis);
            _canvas.Children.Add(yAxis);

            var xAxisArrow = new Path();//x轴箭头
            var yAxisArrow = new Path();//y轴箭头
            xAxisArrow.Fill = Brushes.Black;
            yAxisArrow.Fill = Brushes.Black;
            var xAxisFigure = new PathFigure
            {
                IsClosed = true,
                StartPoint = new Point(_width - xAxis.X1, xAxis.Y1 - 4)                          //路径的起点
            };
            xAxisFigure.Segments.Add(new LineSegment(new Point(_width - xAxis.X1, xAxis.Y1 + 4), false)); //第2个点
            xAxisFigure.Segments.Add(new LineSegment(new Point(_width - xAxis.X1 + 10, xAxis.Y1), false)); //第3个点
            var yAxisFigure = new PathFigure
            {
                IsClosed = true,
                StartPoint = new Point(xAxis.X1 - 4, yAxis.Y1)                          //路径的起点
            };
            yAxisFigure.Segments.Add(new LineSegment(new Point(xAxis.X1 + 4, yAxis.Y1), false)); //第2个点
            yAxisFigure.Segments.Add(new LineSegment(new Point(xAxis.X1, yAxis.Y1 - 10), false)); //第3个点
            var xAxisGeometry = new PathGeometry();
            var yAxisGeometry = new PathGeometry();
            xAxisGeometry.Figures.Add(xAxisFigure);
            yAxisGeometry.Figures.Add(yAxisFigure);
            xAxisArrow.Data = xAxisGeometry;
            yAxisArrow.Data = yAxisGeometry;
            _canvas.Children.Add(xAxisArrow);
            _canvas.Children.Add(yAxisArrow);

            _xMin = xAxis.X1;
            _xMax = xAxis.X2;
            _yMin = yAxis.Y1;
            _yMax = yAxis.Y2;

            for (var i = 0; i < 6; ++i)
            {
                _canvas.Children.Add(_lines[i]);
            }

            //TextBlock xLabel = new TextBlock();
            //TextBlock yLabel = new TextBlock();
            //TextBlock oLabel = new TextBlock();
            //xLabel.Text = "月";
            //yLabel.Text = "元";
            //oLabel.Text = "0";
            //Canvas.SetLeft(xLabel, 610);
            //Canvas.SetLeft(yLabel, 20);
            //Canvas.SetLeft(oLabel, 20);
            //Canvas.SetTop(xLabel, _height / 2.0 - 3);
            //Canvas.SetTop(yLabel, 4);
            //Canvas.SetTop(oLabel, _height / 2.0 - 8);
            //xLabel.FontSize = 14;
            //yLabel.FontSize = 14;
            //oLabel.FontSize = 14;
            //_canvas.Children.Add(xLabel);
            //_canvas.Children.Add(yLabel);
            //_canvas.Children.Add(oLabel);
        }

        public void AddData(MainWindow.MpuData mpuData)
        {
            _bound = 0;
            var mpuDataArray = new double[6];
            mpuDataArray[0]=mpuData.AccX;
            mpuDataArray[1]=mpuData.AccY;
            mpuDataArray[2]=mpuData.AccZ;
            mpuDataArray[3]=mpuData.GyrX;
            mpuDataArray[4]=mpuData.GyrY;
            mpuDataArray[5]=mpuData.GyrZ;
            for (var i = 0; i < 6; ++i)
            {
                if (mpuDataArray[i] > 2000)
                    return;
                if (double.IsNaN(mpuDataArray[i]))
                    return;
                if (double.IsInfinity(mpuDataArray[i]))
                    return;
            }
            

            for (var i = 0; i < 6; ++i)
            {
                _data[i].Add(mpuDataArray[i]);

                while (_data[i].Count > DataLength)
                    _data[i].RemoveAt(0);
                if (!_show[i]) continue;
                for (var j = 0; j < _data[i].Count; ++j)
                {
                    var abs = Math.Abs((double) _data[i][j]);
                    if (abs > _bound)
                        _bound = abs;
                }
            }

            _eularAngle = _calculator.UpdateData(mpuData);
        }

        private double Lerp(double x, double x0, double x1, double y0, double y1)
        {
            return (x - x1) / (x0 - x1) * y0 + (x - x0) / (x1 - x0) * y1;
        }

        public void Update()
        {
            if (!_canvasInitialized)
            {
                InitializeCanvas();
                _canvasInitialized = true;
            }

            for (var i = 0; i < 6; ++i)
            {
                if (!_show[i]) continue;
                var coordinatePoints = new PointCollection();
                for (var j = 0; j < _data[i].Count; ++j)
                {
                    var d = (double)_data[i][j];
                    var xd = Lerp(j, 0, DataLength - 1, _xMin, _xMax);
                    var yd = Lerp(d, -_bound * 1.1, _bound * 1.1, _yMin, _yMax);
                    coordinatePoints.Add(new Point(xd, yd));
                }
                _lines[i].Points = coordinatePoints;
            }

            var rotateX = new RotateTransform3D
            {
                Rotation = new AxisAngleRotation3D(new Vector3D(1, 0, 0), _eularAngle.X)
            };
            var rotateY = new RotateTransform3D
            {
                Rotation = new AxisAngleRotation3D(new Vector3D(0, 1, 0), _eularAngle.Z)
            };
            var rotateZ = new RotateTransform3D
            {
                Rotation = new AxisAngleRotation3D(new Vector3D(0, 0, 1), _eularAngle.Y)
            };
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(rotateX);
            transformGroup.Children.Add(rotateY);
            transformGroup.Children.Add(rotateZ);
            _model.Transform = transformGroup;
        }

        public void SetShow(bool[] show)
        {
            _show = show;
            for (var i = 0; i < 6; ++i)
            {
                _lines[i].StrokeThickness = !show[i] ? 0 : 2;
            }
        }

        public void ResetCalculator()
        {
            _calculator.Reset();

            var rotateZ = new RotateTransform3D
            {
                Rotation = new AxisAngleRotation3D(new Vector3D(0, 1, 0), 180)
            };
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(rotateZ);
            _model.Transform = transformGroup;
        }

        public void Resize()
        {
            for (var i = 0; i < 6; ++i)
                _canvas.Children.Remove(_lines[i]);
            _canvas.Children.Clear();
            _canvasInitialized = false;
        }
    }
}
