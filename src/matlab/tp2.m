function [P, vec, val] = tp2(metodo)

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
tests = 41;

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
		X((i-1)*imgs+j,:) =  reshape(A', 1, numel(A)) ;
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

for i=1:n
	TC(i,:)	 = vec*(X(i,:))';
end


%% IDENTIFICACION CARA

fallos = tests;
results = zeros(tests,2);
for k=1:tests
	% Path de la imagen.
	path = path_base;
	path = strcat(path, int2str(k));
	path = strcat(path,'/');
	path_img = path;
	path_img = strcat(path_img,int2str(10));
	path_img = strcat(path_img,'.pgm');

	%Leo imagen.
	vec_img = imread(path_img);
	% Vectorizo.
	vec_img = reshape(vec_img', 1, numel(vec_img));
	vec_img = double(vec_img)-media;
	vec_img = vec_img*(1/(sqrt(n-1)));
	vec_img = (vec*vec_img')';
	
	% Comparo con todas las imagenes.
	minimo = inf;
	for i=1:pers
		for j=1:imgs
			% Cargo muestra del trainset.
			muestra = TC((i-1)*imgs + j,:);
			% Calculo distancia de la imagen a la muestra.
			distancia = norm(vec_img-muestra);
			% Actualizo.
			if distancia < minimo
				minimo = distancia;
				imagen = i;
			end	
		end
	end

	if imagen == k
		fallos = fallos - 1;
	end
	results(k,1) = k;
	results(k,2) = imagen;
end
aciertos = tests-fallos
fallos
