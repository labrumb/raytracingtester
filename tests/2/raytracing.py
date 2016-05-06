import numpy as np
import matplotlib.pyplot as plt
import sys

w = 512
h = 512

def normalize(x):
    x /= np.linalg.norm(x)
    return x

def intersect_plane(O, D, P, N):
    # Return the distance from O to the intersection of the ray (O, D) with the 
    # plane (P, N), or +inf if there is no intersection.
    # O and P are 3D points, D and N (normal) are normalized vectors.
    denom = np.dot(D, N)
    if np.abs(denom) < 1e-6:
        return np.inf
    d = np.dot(P - O, N) / denom
    if d < 0:
        return np.inf
    return d

def intersect_sphere(O, D, S, R):
    # Return the distance from O to the intersection of the ray (O, D) with the 
    # sphere (S, R), or +inf if there is no intersection.
    # O and S are 3D points, D (direction) is a normalized vector, R is a scalar.
    a = np.dot(D, D)
    OS = O - S
    b = 2 * np.dot(D, OS)
    c = np.dot(OS, OS) - R * R
    disc = b * b - 4 * a * c
    if disc > 0:
        distSqrt = np.sqrt(disc)
        q = (-b - distSqrt) / 2.0 if b < 0 else (-b + distSqrt) / 2.0
        t0 = q / a
        t1 = c / q
        t0, t1 = min(t0, t1), max(t0, t1)
        if t1 >= 0:
            return t1 if t0 < 0 else t0
    return np.inf

def intersect(O, D, obj):
    if obj['type'] == 'plane':
        return intersect_plane(O, D, obj['position'], obj['normal'])
    elif obj['type'] == 'sphere':
        return intersect_sphere(O, D, obj['position'], obj['radius'])

def get_normal(obj, M):
    # Find normal.
    if obj['type'] == 'sphere':
        N = normalize(M - obj['position'])
    elif obj['type'] == 'plane':
        N = obj['normal']
    return N
    
def get_color(obj, M):
    color = obj['color']
    if not hasattr(color, '__len__'):
        color = color(M)
    return color

def trace_ray(rayO, rayD):
    # Find first point of intersection with the scene.
    t = np.inf
    for i, obj in enumerate(scene):
        t_obj = intersect(rayO, rayD, obj)
        if t_obj < t:
            t, obj_idx = t_obj, i
    # Return None if the ray does not intersect any object.
    if t == np.inf:
        return
    # Find the object.
    obj = scene[obj_idx]
    # Find the point of intersection on the object.
    M = rayO + rayD * t
    # Find properties of the object.
    N = get_normal(obj, M)
    color = get_color(obj, M)
    toL = normalize(L - M)
    toO = normalize(O - M)

    col_ray = color
    return obj, M, N, col_ray

def add_sphere(position, radius, color):
    return dict(type='sphere', position=np.array(position), 
        radius=np.array(radius), color=np.array(color), reflection=.5)
    
    
# Light position and color.
L = np.array([5., 5., -10.])
color_light = np.ones(3)

depth_max = 1  # Maximum number of light reflections.
col = np.zeros(3)  # Current color.
O = np.array([0., 0., 0.])  # Camera.
Q = np.array([0., 0., -1.])  # Camera pointing to.
img = np.zeros((h, w, 3))


name=""
scene = []

if __name__ == '__main__':
	with open(sys.argv[1]) as f:
	    for line in f:
		tokens = line.split()
		if len(tokens) == 1:
		    tokens = line.split(",")
	            #get the test name
		    if len(tokens) == 1:
			if line[:1] == "t":
			    name = line + ".png";
			#generate the image		
			elif line[:1] == "e":
			    r = float(w) / h
			    # Screen coordinates: x0, y0, x1, y1.
			    S = (-1., -1. / r + .125, 1., 1. / r + .125)
			    # Loop through all pixels.
			    for i, x in enumerate(np.linspace(S[0], S[2], w)):
			        for j, y in enumerate(np.linspace(S[1], S[3], h)):
			            col[:] = 0
			            Q[:2] = (x, y)
			            D = normalize(Q - O)
			            depth = 0
			            rayO, rayD = O, D
			            reflection = 1.
			            # Loop through initial and secondary rays.
			            while depth < depth_max:
			                traced = trace_ray(rayO, rayD)
			                if not traced:
			                    break
			                obj, M, N, col_ray = traced
			                # Reflection: create a new ray.
			                rayO, rayD = M + N * .0001, normalize(rayD - 2 * np.dot(rayD, N) * N)
			                depth += 1
			                col += reflection * col_ray
			                reflection *= obj.get('reflection', 1.)
			            img[h - j - 1, i, :] = np.clip(col, 0, 1)
			    
			    plt.imsave(name, img)
			    scene =[];
			#get image resolution
			else:
			    w = int(line)
			    h = w
	
		else:
		    location = tokens[2].split(",")
	            color = tokens[4].split(",")
	            radius = tokens[6]
	
	            scene.append(add_sphere([float(location[0]),float(location[1]),float(location[2])], float(radius), [float(color[0]), float(color[1]), float(color[2])]))
