vbus = 24;          % 直流侧母线电压
vn = vbus/sqrt(2);  % 线电压有效

%电机参数
Np = 4;             % 极对数
Rs = 0.45;          % 定子电阻
Ls= 0.00042;        % 定子电感
Ld= 0.00042;        % 定子电感
Lq= 0.00042;        % 定子电感
Flux = 0.00525;   % 磁链

Jx = 0.000004483738;% 转动惯量
delta = 4;          % 阻尼因子
Fcoef = 0;
spd_max = 1000;
P = 120;
Te = P/(spd_max/Np);
KT = 1.5*Np*Flux;

% CurrentLoop PI
CurrentLoopBandwidth = 500*2*pi;
id_kp = Ls*CurrentLoopBandwidth; 
id_ki = Rs/Ls;
vd_limit = vbus/sqrt(3);

iq_kp = Ls*CurrentLoopBandwidth;
iq_ki = Rs/Ls;
vq_limit = vbus/sqrt(3);
CurrentLoopMax = vbus/sqrt(3);

iq_kp = 20;
iq_ki = 10;

id_kp = 2;
id_ki = 100;
