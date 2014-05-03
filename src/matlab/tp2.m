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
iter = 100; % Cantidad de iteraciones para el metodo de potencias.
x = ones(m,1); % Vector inicial para el metodo de la potencias.

res = zeros(n,m);
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
%% Vectorizar, Hacer esto de forma mas MATLAB
%		res((i-1)*imgs+j,:) = double(A(:)');	Esto no funcion, probar otra cosa.
% Copio a res
		count = 1;
		for k=1:alto
			for h=1:ancho
				res((i-1)*imgs+j,count) = A(k,h);
				count = count+1;
			end
		end
	end
end

% Calculo covarianza.
for j=1:m
	media = 0;
	for i=1:n
		media = media + res(i,j);
	end
	media = media / n;
	for i=1:n,
		res(i,j) = res(i,j) - media;
	end

end

res = res * (1/sqrt(n-1));

% Metodo normal.
if metodo == 0
	A = res'*res;

	% Metodo potencia y deflacion.
	for i=1:comp
		[avec,landa] = pim(x,A, iter);
		vec(i,:) =  avec';
		val(i,:) = landa;
		A = def(A, avec, landa);
	end
end

% Metodo alternativo.
if metodo == 1
	A = res*res';

	for i=1:comp
		[avec,landa] = pim(y,A, iter);
		pvec(i,:) =  avec';
		val(i,:) = landa;
		A = def(A, avec, landa);
	end

	for i=1:comp
		vec(i,:) = (sqrt(val(i,:))/val(i,:))*res'*pvec(i,:)';
	end
	for i=1:comp
		val(i,:) = sqrt(val(i,:));
	end
end
