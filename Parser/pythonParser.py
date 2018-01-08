import sys

#receive the file to parse
fileName    = sys.argv[1] #name of the file to read
id          = int(sys.argv[2]) #initial id for the game objects to be created

#open read file and write file
objFile = open(fileName, 'r')

destFile = open(fileName+'.xml', 'w')

#load arrays for vertex and faces
vertexArray = []
faceArray = []

vertexArray.append(0)   #insert and initial 0 value, in this way we make
                        #that vertex and face numeration matches

#read and store data from obj file
for line in objFile:
    if line[0] == 'v' and line[1] == ' ' :
        #split vertex into x,y,z and push them into vertex array
        aux = line.split(' ')
        vec3 = []
        vec3.append(float(aux[1]))
        vec3.append(float(aux[2]))
        vec3.append(float(aux[3]))
        vertexArray.append(vec3)
    elif line[0] == 'f' and line[1] == ' ' :
        aux = line.split(' ')
        vec4 = []
        vec4.append(int(aux[1].split('/')[0]))
        vec4.append(int(aux[2].split('/')[0]))
        vec4.append(int(aux[3].split('/')[0]))
        vec4.append(int(aux[4].split('/')[0]))
        faceArray.append(vec4)

#close obj file once read
objFile.close()

#reorder vertex in faces depending on its x and z values
for i, face in enumerate(faceArray):

    min_x = 2147483647
    max_z = -2147483647
    nFace = 0
    
    for vertex in face:
        if min_x >= vertexArray[vertex][0]: #check for the value that is most in the left

            if min_x == vertexArray[vertex][0]: #if another point has the same x
                if max_z < vertexArray[vertex][2]: #go for the higher z
                    min_x = vertexArray[vertex][0]
                    max_z = vertexArray[vertex][2]
                    nFace = vertex 
            else:
                min_x = vertexArray[vertex][0]  #if it's the lower x, save the value
                max_z = vertexArray[vertex][2]
                nFace = vertex
            
    #once we have the starting vertex, we rotate each vector to have 
    #the starting vertex in its start

    #firstly we search for the position in with it's flaced the starting face
    faceStart = 0
    for faceStart, vertex in enumerate(face):
        if nFace == vertex:
            break

    #then we loop over the array reallocating the data
    auxFace = []
    for idx in range(0,4):
        auxFace.append(face[faceStart])
        if faceStart == 3:
            faceStart = 0
        else:
            faceStart += 1

    #then we reassign the face
    faceArray[i] = auxFace

#make an array that holds for every face, which other faces have vertex in common with it
relatedFaces = [None] * len(faceArray)
for i, iter in enumerate(relatedFaces):
    relatedFaces[i] = []    #create empty 2D array
for i, face in enumerate(faceArray):    #we iterate for every face with all other faces
    for j, oFace in enumerate(faceArray):
        if i != j:                      #if we aren't comparing with the same plane 
            occurence = 0
            for vertex in faceArray[i]:  #we compare every vertex of our face to the vertex of the other faces
                if vertex == faceArray[j][0]  or vertex == faceArray[j][1] or vertex == faceArray[j][2] or vertex == faceArray[j][3]:
                    occurence += 1
            if occurence > 1:
                    relatedFaces[i].append(j) #if there's a vertex in common, add j

#for every edge of every face, mark connected edges or -1 if none
relatedEdges = [None] * len(faceArray)
for i, iter in enumerate(relatedEdges):
    relatedEdges[i] = [str(-1),str(-1),str(-1),str(-1)]
for i, face in enumerate(relatedFaces): #for every face
    for faceId in face: #check faces connecting with it
        for j in range(0, 4):    #for every line of our face
            aux = j + 1 
            if aux == 4:
                aux = 0
                
            for z in range(0, 4):   #compare with all the other four lines
                aux1 = z + 1 
                if aux1 == 4:
                    aux1 = 0

                #if a pair of points is related to another
                if(faceArray[i][j] == faceArray[faceId][z] or faceArray[i][j] == faceArray[faceId][aux1]) and \
                  (faceArray[i][aux] == faceArray[faceId][z] or faceArray[i][aux] == faceArray[faceId][aux1]):
                    relatedEdges[i][j] = str(id+faceId) + ':' + str(z)

#write the xml
for i, face in enumerate(faceArray):

    p1 = 'p1=\"' + str(vertexArray[face[0]][0]) + ',' + str(vertexArray[face[0]][1]) + ',' + str(vertexArray[face[0]][2]) + '\" '
    p2 = 'p2=\"' + str(vertexArray[face[1]][0]) + ',' + str(vertexArray[face[1]][1]) + ',' + str(vertexArray[face[1]][2]) + '\" '
    p3 = 'p3=\"' + str(vertexArray[face[2]][0]) + ',' + str(vertexArray[face[2]][1]) + ',' + str(vertexArray[face[2]][2]) + '\" ' 
    p4 = 'p4=\"' + str(vertexArray[face[3]][0]) + ',' + str(vertexArray[face[3]][1]) + ',' + str(vertexArray[face[3]][2]) + '\" '

    pos = '\"' + str(vertexArray[face[0]][0]) + ',' + str(vertexArray[face[0]][1]) + ',' + str(vertexArray[face[0]][2]) + '\" '
    p0  = '\"' + str(0) + ',' + str(0) + ',' + str(0) + '\" '

    gameObject          =  '<object id=\"' + str(id+i) + '\" pos=' + pos + ' rot=' + p0 + ' sca=' + p0 + '>\n'
    terrainComponent    =  '    <component name=\"terrain\"' + ' l0=\"' + relatedEdges[i][0] + '\" l1=\"' + relatedEdges[i][1] + '\" l2=\"' + relatedEdges[i][2] + '\" l3=\"' + relatedEdges[i][3] + '\">\n'
    bbox                =  '        <bbox ' + p1 + p2 + p3 + p4 + 'friction=\"0.2\" />\n'
    terrainComponentEnd =  '    </component>\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(terrainComponent)
    destFile.write(bbox)
    destFile.write(terrainComponentEnd)
    destFile.write(gameObjectEnd)

#close write file
destFile.close()

