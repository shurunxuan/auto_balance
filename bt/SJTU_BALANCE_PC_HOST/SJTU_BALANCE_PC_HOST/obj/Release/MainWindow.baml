   M S B A M L   `   `   `  ���� O  KSJTU_BALANCE_PC_HOST, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null;"clr-namespace:SJTU_BALANCE_PC_HOSTSJTU_BALANCE_PC_HOST  O Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem %    SJTU_BALANCE_PC_HOST.MainWindow   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     4local"clr-namespace:SJTU_BALANCE_PC_HOST     9system&clr-namespace:System;assembly=mscorlib     -     �Name$  Window�� � Title$ 
MainWindow��$��796.877��$��1070.848��.���  � RowDefinitions �� $
��224* ��� $
��105* ��� $
��201* ��� $
��240* � � ColumnDefinitions �� �� �� $��3* �.+��� $ ��Balance Car Bluetooth Port��$��10,10,9.5,10q�$��0��$��0��.��� $��10q� �� $��2* ��� $��3* � �� �� �� .+��� -    ��Name$ PortNameCombo��$��0q�$��22��$��Center=� �� Text$
 COM8��$��0��$��1���� -    ��Name$ OpenPortButton��$��	Open Port��$��22��$��0q�$��Center=�{�.  $��2��$��2��$��0���� $
��Left=�$��0q�$��	Port Name��$��Center=�$��0q�$��0��$��0����Z� �� $
��Left=�$��0q�$��	Baud Rate��$��Center=�$��0q�$��1��$��0����Z� �� -   $ BaudRateCombo��$��0q�$��22��$��Center=� �� SelectedIndex$ 1��$��1��$��1��.@���4800����9600����	19200����	38400����	57200����
115200���� $��Basic Functions��$��10,10,9.5,10.5q�$��1��$��2��$��0��.��� $��10q� �� �� �� �� �� �� �� �� �� .+��� -   $ CheckVersionButton��$��Check Version��$��0q�$��Center=�$��0���� -   $ GetAccXButton��$��
Get ACC__X��$��0q�$��Center=�$��1���� -   $ GetAccYButton��$��
Get ACC__Y��$��0q�$��Center=�$��2���� -   $ GetAccZButton��$��
Get ACC__Z��$��0q�$��Center=�$��3���� -	   $ GetGyrXButton��$��
Get GYR__X��$��0q�$��Center=�$��4���� -
   $ GetGyrYButton��$��
Get GYR__Y��$��0q�$��Center=�$��5���� -   $ GetGyrZButton��$��
Get GYR__Z��$��0q�$��Center=�$��6���� -   $ GetAllMpuDataButton��$��Get All MPU Data��$��0q�$��Center=�$��7���� -    ��Name$ AutoUpdateCheckBox��$��Auto Update MPU Data��$��Center=�$��20��$��0q�$��Center=�$	��164��$��Center=�$��8���� $��MPU-6050 Status��$��10.5,10,10.5,11q�$��1��$��0��$��2��.��� $��10q� �� �� �� �� �� ��  �� $��2* ��� $��3* �.+��� $��Right=�$��22��$��0,0,10,0q�$��ACC__X��$��Center=�$��Right=�$��Center=�$��0q�$��0��$��0����Z� �� $��Right=�$��22��$��0,0,10,0q�$��ACC__Y��$��Center=�$��Right=�$��Center=�$��0q�$��1��$��0����Z� �� $��Right=�$��22��$��0,0,10,0q�$��ACC__Z��$��Center=�$��Right=�$��Center=�$��0q�$��2��$��0����Z� �� $��Right=�$��22��$��0,0,10,0q�$��GYR__X��$��Center=�$��Right=�$��Center=�$��0q�$��3��$��0����Z� �� $��Right=�$��22��$��0,0,10,0q�$��GYR__Y��$��Center=�$��Right=�$��Center=�$��0q�$��4��$��0����Z� �� $��Right=�$��22��$��0,0,10,0q�$��GYR__Z��$��Center=�$��Right=�$��Center=�$��0q�$��5��$��0����Z� �� -   	 ��Name$	 	AccXLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��0��$��1����Z� �� -   $	 	AccYLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��1��$��1����Z� �� -   $	 	AccZLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��2��$��1����Z� �� -   $	 	GyrXLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��3��$��1����Z� �� -   $	 	GyrYLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��4��$��1����Z� �� -   $	 	GyrZLabel��$
��Left=�$��22��$��10,0,0,0q�$�� ��$��Center=�$��Center=�$��0q�$��5��$��1����Z� �� $��MPU-6050 Status��$��9.5,10,10,10.5q�$��2��$��0��$��3��.��� $��10q� �� �� �� �� �� ��  �� $��6* ��� $��1* �.+��� -   
 ��Name$
 	MpuCanvas��$��0q�$��Disabled=�$��6���� -   $ LegendCheckBox0��$��ACC__X��$
��Left=�$��10q�$��Center=�$��Center=� d� 	IsChecked$
 TrueY�$��1��$��0���� -   $ LegendCheckBox1��$��ACC__Y��$
��Left=�$��10q�$��Center=�$��Center=�$
 TrueY�$��1��$��1���� -   $ LegendCheckBox2��$��ACC__Z��$
��Left=�$��10q�$��Center=�$��Center=�$
 TrueY�$��1��$��2���� -   $ LegendCheckBox3��$��GYR__X��$
��Left=�$��10q�$��Center=�$��Center=�$
 TrueY�$��1��$��3���� -   $ LegendCheckBox4��$��GYR__Y��$
��Left=�$��10q�$��Center=�$��Center=�$
 TrueY�$��1��$��4���� -   $ LegendCheckBox5��$��GYR__Z��$
��Left=�$��10q�$��Center=�$��Center=�$
 TrueY�$��1��$��5���� $��Car Posture��$��10.5,9,10.5,10.5q�$��1��$��2��.��� $��10q�.+�'� -    '�Name$ PostureViewport��$��0q� '� Camera =�  � FarPlaneDistance$ 20Q� � LookDirection$ 0,-0.5,35� � UpDirection$ 0,1,05� � NearPlaneDistance$ 1Q� � Position$ 0,0.5,-31� =� FieldOfView$ 45Q�x�g�  g� Content h� ��t�  �� Color$ 	#FFFFFFFF�� t� 	Direction$ 3,-4,55��� $ 	OrangeRed��� -   	Model�� � Geometry k�  k� 	Positions� �   �^��`y�� ��� � `y�� ��� � ��  ����^����  ����^����  5 �^��`y�� 5  � `y�� 5  � ��  5  k� TriangleIndices. �$          � Material u�  u� Brush ��  �� Color$ Yellow�� �� Opacity$	 1.0Q��� -   $ PostureResetButton��$��Reset��$��Right=�$��25��$��0q�$	��Top=�$��64���� $��
Parameters��$��10,9.5,9.5,9.5q�$��3��$��0��$��2��.��� $��10q� �� �� ��  �� �� �� �� �� �� .+��� -   $ GetBalanceKpButton��$��Get Balance k__p��$��0,0,5,0q�$��Center=�$��0��$��0���� -   $ GetBalanceKiButton��$��Get Balance k__d��$��0,0,5,0q�$��Center=�$��1��$��0���� -    $ GetVelocityKpButton��$��Get Velocity k__p��$��0,0,5,0q�$��Center=�$��2��$��0���� -!   $ GetVelocityKiButton��$��Get Velocity k__i��$��0,0,5,0q�$��Center=�$��3��$��0���� -"   $ GetAngleButton��$��	Get Angle��$��0,0,5,0q�$��Center=�$��4��$��0���� -#   $��BalanceKpTextBox��$��5,0,5,0q� �� TextWrapping$
 Wrap=�$��0��$��Center=�$��0��$��1���� -$   $��BalanceKiTextBox��$��5,0,5,0q�$
 Wrap=�$��0��$��Center=�$��1��$��1���� -%   $��VelocityKpTextBox��$��5,0,5,0q�$
 Wrap=�$��0��$��Center=�$��2��$��1���� -&   $��VelocityKiTextBox��$��5,0,5,0q�$
 Wrap=�$��0��$��Center=�$��3��$��1���� -'   $��AngleTextBox��$��5,0,5,0q�$
 Wrap=�$��0��$��Center=�$��4��$��1���� -(   $��SpeedTextBox��$��5,0,5,0q�$
 Wrap=�$��0��$��Center=�$��5��$��0��$��2���� -)   $ SetBalanceKpButton��$��Set Balance k__p��$��0,0,5,0q�$��Center=�$��0��$��2���� -*   $ SetBalanceKiButton��$��Set Balance k__d��$��0,0,5,0q�$��Center=�$��1��$��2���� -+   $ SetVelocityKpButton��$��Set Velocity k__p��$��0,0,5,0q�$��Center=�$��2��$��2���� -,   $ SetVelocityKiButton��$��Set Velocity k__i��$��0,0,5,0q�$��Center=�$��3��$��2���� --   $ SetAngleButton��$��	Set Angle��$��0,0,5,0q�$��Center=�$��4��$��2���� -.   $ SetSpeedButton��$��	Set Speed��$��0,0,5,0q�$��Center=�$��5��$��2��