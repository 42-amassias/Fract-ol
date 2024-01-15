#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// #define MAX_ITR 1024

#define WIDTH 1920
#define HEIGHT 1080

#define DEFAULT_COLOR 0x000000

#define LOG2 0.6931471805599453

#define PARAM__A 1.4426950408889634 // 1. / log(2.)
#define PARAM__B 0.3400464821989298 // 1. / (3. * sqrt(2.) * log(2.))
#define PARAM__C 0.1796537728450938 // 1. / (3. * pow(3, 1. / 8.) * log(2.))

#define THRESHOLD 1000000.

#define THICKNESS_FACTOR .03

inline int	lerp(
					int a,
					int b,
					double t)
{
	char	*_a;
	char	*_b;
	int		color;

	_a = (char *)&a;
	_b = (char *)&b;
	color = 0x000000;
	for (int i = 0; i < 3; ++i)
		((char *)&color)[i] = (char) ((double) _a[i] + t * ((double) _b[i] - (double) _a[i]));
	return (color);
}

inline int	f(
		double potential)
{
	int	color;

	color = 0x000000;
	color |= (0x7F - (int) (0x7F * cos(potential * PARAM__A + 1))) << 16;
	color |= (0x7F - (int) (0x7F * cos(potential * PARAM__B + 1))) << 8;
	color |= (0x7F - (int) (0x7F * cos(potential * PARAM__C + 1))) << 0;
	return (color);
}

__kernel void	julia_toulouse(
					__global int *screen,
					uint width,
					uint height,
					double dx,
					double dy,
					double zoom,
					double cx,
					double cy,
					int max_itr,
					double k
					)
{
	int		x;
	int		y;

	double	zx;
	double	zy;
	double	dcx;
	double	dcy;

	int		itr;

	int		color;

	bool	inside;

	double	R2;
	double	v;
	double	d;

	double	tmp;
	double	zx2;
	double	zy2;

	x = get_global_id(0);
	y = get_global_id(1);

	zx = 4. * ((double) x / ((double) (WIDTH - 1))) - 2.;
	zy = 4. * ((double) y / ((double) (HEIGHT - 1))) - 2.;

	zx *= ((double) WIDTH) / ((double) HEIGHT);

	zx = zx / zoom + dx;
	zy = zy / zoom + dy;
	zx2 = zx * zx;
	zy2 = zy * zy;
	R2 = zx2 + zy2;

	dcx = 1.;
	dcy = 0.;

	itr = 0;

	inside = true;

	while (itr++ < max_itr)
	{
		if (R2 > THRESHOLD)
		{
			inside = false;
			break ;
		}

		// dc = 2 * dc * z + 1
		tmp = dcx;
		dcx = 2. * (tmp * zx - dcy * zy) + 1;
		dcy = 2. * (tmp * zy + dcy * zx);

		// z = z*z + c
		tmp = zx;
		zx = zx2 - zy2 + cx;
		zy = 2. * tmp * zy + cy;

		zx2 = zx * zx;
		zy2 = zy * zy;
		R2 = zx2 + zy2;
	}

	color = DEFAULT_COLOR;
	if (!inside)
	{
		tmp = log(R2);
		if (R2 * tmp * tmp < THICKNESS_FACTOR * THICKNESS_FACTOR * (dcx * dcx + dcy * dcy))
		{
			v = log(tmp) - LOG2 * (double)itr;
			color = f(v / k);
		}
		// R2 = sqrt(R2);
		// tmp = 2. * R2 * log(R2) / sqrt(dcx * dcx + dcy * dcy);
		// d = tmp / THICKNESS_FACTOR;
		// if (d > 1.)
		// 	d = 1.;
		// v = log(tmp) - LOG2 * (double)itr;
		// color = lerp(f(v / k), DEFAULT_COLOR, d);
	}

	screen[x + WIDTH * y] = color;
}

// ---- ---- ??? ---- ----- //
// x = -1.253583
// y = 0.382563
// ---- ---- ??? ---- ----- //

// ---- ---- ---- ---- ----  THE SQUARE ---- ---- ---- ---- ---- //
// Current kernel's arguments :
//         * double          dx              = -0.782385
//         * double          dy              = -0.137764
//         * double          zoom            = 15934088.227282
//         * int             max_itr         = 1024
//         * double          k               = 0.900000
// ---- ---- ---- ---- ----  THE SQUARE ---- ---- ---- ---- ---- //

__kernel void	mandelbrot_toulouse(
					__global int *screen,
					uint width,
					uint height,
					double dx,
					double dy,
					double zoom,
					int max_itr,
					double k
					)
{
	int		x;
	int		y;

	double	cx;
	double	cy;

	double	zx;
	double	zy;
	double	dcx;
	double	dcy;

	int		itr;

	int		color;

	bool	inside;

	double	R2;
	double	v;
	double	d;

	double	tmp;
	double	zx2;
	double	zy2;

	x = get_global_id(0);
	y = get_global_id(1);

	cx = 4. * ((double) x / ((double) (WIDTH - 1))) - 2.;
	cy = 4. * ((double) y / ((double) (HEIGHT - 1))) - 2.;

	cx *= ((double) WIDTH) / ((double) HEIGHT);

	cx = cx / zoom + dx;
	cy = cy / zoom + dy;

	zx = cx;
	zy = cy;
	zx2 = zx * zx;
	zy2 = zy * zy;
	R2 = zx2 + zy2;

	dcx = 1.;
	dcy = 0.;

	itr = 0;

	inside = true;

	while (itr++ < max_itr)
	{
		if (R2 > THRESHOLD)
		{
			inside = false;
			break ;
		}

		// dc = 2 * dc * z + 1
		tmp = dcx;
		dcx = 2. * (tmp * zx - dcy * zy) + 1;
		dcy = 2. * (tmp * zy + dcy * zx);

		// z = z*z + c
		tmp = zx;
		zx = zx2 - zy2 + cx;
		zy = 2. * tmp * zy + cy;

		zx2 = zx * zx;
		zy2 = zy * zy;
		R2 = zx2 + zy2;
	}

	color = DEFAULT_COLOR;
	if (!inside)
	{
		tmp = log(R2);
		if (R2 * tmp * tmp < THICKNESS_FACTOR * THICKNESS_FACTOR * (dcx * dcx + dcy * dcy))
		{
			v = log(tmp) - LOG2 * (double)itr;
			color = f(v / k);
		}
		// R2 = sqrt(R2);
		// tmp = 2. * R2 * log(R2) / sqrt(dcx * dcx + dcy * dcy);
		// d = tmp / THICKNESS_FACTOR;
		// if (d > 1.)
		// 	d = 1.;
		// v = log(tmp) - LOG2 * (double)itr;
		// color = lerp(f(v / k), DEFAULT_COLOR, d);
	}

	screen[x + WIDTH * y] = color;
}

#define THICKNESS_FACTOR 10

__kernel void	burning_ship_toulouse(
					__global int *screen,
					uint width,
					uint height,
					double dx,
					double dy,
					double zoom,
					int max_itr,
					double k
					)
{
	int		x;
	int		y;

	double	cx;
	double	cy;

	double	zx;
	double	zy;
	double	dcx;
	double	dcy;

	int		itr;

	int		color;

	bool	inside;

	double	R2;
	double	v;
	double	d;

	double	tmp;
	double	zx2;
	double	zy2;

	x = get_global_id(0);
	y = get_global_id(1);

	cx = 4. * ((double) x / ((double) (WIDTH - 1))) - 2.;
	cy = 4. * ((double) y / ((double) (HEIGHT - 1))) - 2.;

	cx *= ((double) WIDTH) / ((double) HEIGHT);

	cx = cx / zoom + dx;
	cy = cy / zoom + dy;

	zx = cx;
	zy = cy;
	zx2 = zx * zx;
	zy2 = zy * zy;
	R2 = zx2 + zy2;

	dcx = 1.;
	dcy = 0.;

	itr = 0;

	inside = true;

	while (itr++ < max_itr)
	{
		if (R2 > THRESHOLD)
		{
			inside = false;
			break ;
		}

		// dc = 2 * dc * z + 1
		tmp = dcx;
		dcx = 2. * (tmp * zx - dcy * zy) + 1;
		dcy = 2. * (tmp * zy + dcy * zx);

		// z = z*z + c
		tmp = zx;
		zx = zx2 - zy2 + cx;
		zy = 2. * fabs(tmp) * fabs(zy) + cy;
		zx2 = zx * zx;
		zy2 = zy * zy;
		R2 = zx2 + zy2;
	}

	color = DEFAULT_COLOR;
	if (!inside)
	{
		tmp = log(R2);
		if (R2 * tmp * tmp < THICKNESS_FACTOR * THICKNESS_FACTOR * (dcx * dcx + dcy * dcy))
		{
			v = log(tmp) - LOG2 * (double)itr;
			color = f(v / k);
		}
	}

	screen[x + WIDTH * y] = color;
}