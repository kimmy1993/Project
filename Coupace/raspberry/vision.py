"""
    usage> ./face.py <path-to-image>
"""

# [START import_libraries]
import argparse
import base64
import httplib2
import logging
import json

from googleapiclient import discovery
from oauth2client.service_account import ServiceAccountCredentials
from httplib2 import Http

# The url template to retrieve the discovery document for trusted testers.
DISCOVERY_URL='https://{api}.googleapis.com/$discovery/rest?version={apiVersion}'
# [END import_libraries]


logging.basicConfig(level=logging.INFO)

def main(photo_file):
    """Run a label request on a single image"""

    # [START authenticate]
    # credentials = GoogleCredentials.get_application_default()
    photo_file="./buf/set.png"
    scopes = ['https://www.googleapis.com/auth/cloud-platform']
    credentials = ServiceAccountCredentials.from_json_keyfile_name('/work/chaosnet/coupaceCloud-f23801995b13.json', scopes)
    #credentials = ServiceAccountCredentials.from_json_keyfile_name('coupaceCloud-f23801995b13.json', scopes)
    http_auth = credentials.authorize(Http())
    service = discovery.build('vision', 'v1', credentials=credentials,discoveryServiceUrl=DISCOVERY_URL)

    # [END authenticate]

    # [START construct_request]

    with open(photo_file, 'rb') as image:
	    image_content = base64.b64encode(image.read())

	    batch_request = [{
		'image': {
		    'content': image_content.decode('UTF-8')
		    },
		'features': [{
		    'type': 'FACE_DETECTION',
		    'maxResults': 1,
		    }]
		}]

	    request = service.images().annotate(body={
		'requests': batch_request,
		})
	    response = request.execute()
	    
	    face = response['responses'][0]['faceAnnotations']
	    """
	    print('%s : %s' % ('sorrow',face[0]['sorrowLikelihood']))
	    print('%s : %s' % ('anger',face[0]['angerLikelihood']))
	    print('%s : %s' % ('surprise',face[0]['surpriseLikelihood']))
	    print('raw data:')
	    print('Found face_detect: %s for %s' % (face, photo_file))
	    
	    if( ( (face[0]['joyLikelihood'])=='POSSIBLE' ) or ( (face[0]['joyLikelihood'])=='LIKELY' ) or ( (face[0]['joyLikelihood'])=='VERY_LIKELY') ):
                        print('%s : %s' % ('joy',face[0]['joyLikelihood']))
                        print("like")
                        return 2
                    """
	    print('%s : %s' % ('joy',face[0]['joyLikelihood']))
	    return (face[0]['joyLikelihood'])

    # [END parse_response]

# [START run_application]
if __name__ == '__main__':
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('image_file', help='The image you\'d like to label.')
    args = parser.parse_args()
    """
    main(1)
# [END run_application]

