import sys

#receive the file to parse
fileName    = sys.argv[1] #name of the file to read

#open read file and write file
objFile = open(fileName, 'r')

destFile = open("./../Game/media/xml/circuit.xml", 'w')

#load arrays for vertex and faces
TVertexArray = []
TFaceArray = []
TFirstFace = 2147483647
TId = 1000

TVertexArray.append(0)   #insert and initial 0 value, in this way we make
                        #that vertex and face numeration matches
Terrain     = False
Waypoint    = False
Light       = False 
ItemBox     = False
Ramp        = False
Object      = False
StartLine   = False

#read and store data from obj file
for line in objFile:
        #Here we detect which type of object we are reading
    if line[0] == 'o' :
        Terrain     = False
        Waypoint    = False
        Light       = False 
        ItemBox     = False
        Ramp        = False
        Object      = False
        StartLine   = False

        if line[2] == 'T' :
            Terrain         = True
        if line[2] == 'W' :
            Waypoint        = True
        if line[2] == 'L' :
            Light           = True
        if line[2] == 'I' :
            ItemBox         = True
        if line[2] == 'R' :
            Ramp            = True
        if line[2] == 'O' :
            Object          = True
        if line[2] == 'S' :
            StartLine       = True

    #Here we store data for our terrain
    if Terrain == True and line[0] == 'v' and line[1] == ' ' :

        #split vertex into x,y,z and push them into vertex array
        aux = line.split(' ')
        vec3 = []
        vec3.append(float(aux[1]))
        vec3.append(float(aux[2]))
        vec3.append(float(aux[3]))
        TVertexArray.append(vec3)
    elif Terrain == True and line[0] == 'f' and line[1] == ' ' :
        aux = line.split(' ')
        vec4 = []
        vec4.append(int(aux[1].split('/')[0]))
        vec4.append(int(aux[2].split('/')[0]))
        vec4.append(int(aux[3].split('/')[0]))
        vec4.append(int(aux[4].split('/')[0]))
        TFaceArray.append(vec4)

        #get the first face in order to index correctly
        if TFirstFace > int(aux[1].split('/')[0]) - 1 :
            TFirstFace = int(aux[1].split('/')[0]) - 1
        if TFirstFace > int(aux[2].split('/')[0]) - 1 :
            TFirstFace = int(aux[2].split('/')[0]) - 1
        if TFirstFace > int(aux[3].split('/')[0]) - 1 :
            TFirstFace = int(aux[3].split('/')[0]) - 1
        if TFirstFace > int(aux[4].split('/')[0]) - 1 :
            TFirstFace = int(aux[4].split('/')[0]) - 1

#close obj file once read
objFile.close()

#reorder vertex in faces depending on its x and z values
for i, face in enumerate(TFaceArray):

    min_x = 2147483647
    max_z = -2147483647
    nFace = 0

    for vertex in face:
        
        #Get correct index
        vtx = vertex-TFirstFace

        if min_x >= TVertexArray[vtx][0]: #check for the value that is most in the left

            if min_x == TVertexArray[vtx][0]: #if another point has the same x
                if max_z < TVertexArray[vtx][2]: #go for the higher z
                    min_x = TVertexArray[vtx][0]
                    max_z = TVertexArray[vtx][2]
                    nFace = vertex 
            else:
                min_x = TVertexArray[vtx][0]  #if it's the lower x, save the value
                max_z = TVertexArray[vtx][2]
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
    TFaceArray[i] = auxFace

#make an array that holds for every face, which other faces have vertex in common with it
relatedFaces = [None] * len(TFaceArray)
for i, iter in enumerate(relatedFaces):
    relatedFaces[i] = []    #create empty 2D array
for i, face in enumerate(TFaceArray):    #we iterate for every face with all other faces
    for j, oFace in enumerate(TFaceArray):
        if i != j:                      #if we aren't comparing with the same plane 
            occurence = 0
            for vertex in TFaceArray[i]:  #we compare every vertex of our face to the vertex of the other faces
                if vertex == TFaceArray[j][0]  or vertex == TFaceArray[j][1] or vertex == TFaceArray[j][2] or vertex == TFaceArray[j][3]:
                    occurence += 1
            if occurence > 1:
                    relatedFaces[i].append(j) #if there's a vertex in common, add j

#for every edge of every face, mark connected edges or -1 if none
relatedEdges = [None] * len(TFaceArray)
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
                if(TFaceArray[i][j] == TFaceArray[faceId][z] or TFaceArray[i][j] == TFaceArray[faceId][aux1]) and \
                  (TFaceArray[i][aux] == TFaceArray[faceId][z] or TFaceArray[i][aux] == TFaceArray[faceId][aux1]):
                    relatedEdges[i][j] = str(TId+faceId) + ':' + str(z)

#write the xml
for i, face in enumerate(TFaceArray):

    p1 = 'p1=\"' + str(TVertexArray[face[0]-TFirstFace][0]) + ',' + str(TVertexArray[face[0]-TFirstFace][1]) + ',' + str(TVertexArray[face[0]-TFirstFace][2]) + '\" '
    p2 = 'p2=\"' + str(TVertexArray[face[1]-TFirstFace][0]) + ',' + str(TVertexArray[face[1]-TFirstFace][1]) + ',' + str(TVertexArray[face[1]-TFirstFace][2]) + '\" '
    p3 = 'p3=\"' + str(TVertexArray[face[2]-TFirstFace][0]) + ',' + str(TVertexArray[face[2]-TFirstFace][1]) + ',' + str(TVertexArray[face[2]-TFirstFace][2]) + '\" ' 
    p4 = 'p4=\"' + str(TVertexArray[face[3]-TFirstFace][0]) + ',' + str(TVertexArray[face[3]-TFirstFace][1]) + ',' + str(TVertexArray[face[3]-TFirstFace][2]) + '\" '

    pos = '\"' + str(TVertexArray[face[0]-TFirstFace][0]) + ',' + str(TVertexArray[face[0]-TFirstFace][1]) + ',' + str(TVertexArray[face[0]-TFirstFace][2]) + '\" '
    p0  = '\"' + str(0) + ',' + str(0) + ',' + str(0) + '\" '

    gameObject          =  '<object id=\"' + str(TId+i) + '\" pos=' + pos + ' rot=' + p0 + ' sca=' + p0 + '>\n'
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

