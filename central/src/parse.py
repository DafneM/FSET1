import json

def json_to_object(message):
    meu_objeto = json.loads(message)

    return meu_objeto

def object_to_json(message):
    
    json_string = json.dumps(message, sort_keys=True, indent=4)
    
    return json_string