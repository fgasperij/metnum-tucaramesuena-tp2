function [vec, val] = tp2()

% Modificar path_root, path_base, alto, ancho, imgs, pers, comp.
% TODO: Parsear los parametros directamente del archivo.
path_root = '/home/user/mn/tp2/';
path_base = 'data/ImagenesCarasRed/s';
alto = 28;
ancho = 23;
imgs = 5;
pers = 41;
comp = 15;
m = ancho*alto;
n = imgs*pers;

%Otros parametros
iter = 300; % Cantidad de iteraciones para el metodo de potencias.
x = ones(m,1); % Vector inicial para el metodo de la potencias.
y = ones(n,1); %Idem superior pero para el metodo alternativo.

X = zeros(n,m);
path_base = strcat(path_root, path_base);
for i=1:pers
	path = path_base;
	path = strcat(path, int2str(i));
	path = strcat(path,'/');
	for j=1:imgs
		path_img = path;
		path_img = strcat(path_img,int2str(j));
		path_img = strcat(path_img,'.pgm');
		A = imread(path_img);
		X((i-1)*imgs+j,:) = A(:)';
	end
end

% Calculo covarianza.
for j=1:m
	mu = 0;
	for i=1:n
		mu = mu + X(i,j);
	end
	mu = mu / n;
	media(j) = mu;
	for i=1:n,
		X(i,j) = X(i,j) - mu;
	end

end

X = X * (1/sqrt(n-1));

% Metodo normal.
if metodo == 0
	A = X'*X;
	% Metodo potencia y deflacion.
	for i=1:comp
		[avec,landa] = pim(x, A, iter);
		vec(i,:) =  avec';
		val(i) = landa;
		A = def(A, avec, landa);
	end

end

% Metodo alternativo.
if metodo == 1
	A = X*X';
	for i=1:comp
		[avec,landa] = pim(y,A, iter);
		pvec(i,:) =  avec';
		val(i) = landa;
		A = def(A, avec, landa);
	end

	for i=1:comp
		vec(i,:) = (sqrt(val(i))/val(i))*X'*pvec(i,:)';
	end

end

for i=1:comp
	val(i) = sqrt(val(i));
end

val = val';


%%Aplicar TC
