using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
public class UDP_server : MonoBehaviour
{
    // Start is called before the first frame update
    [DllImport("UDP", EntryPoint = "RunServer")]
    private static extern void RunServer();
    void Start()
    {
        Vector3 pos = transform.position;
        Debug.Log(pos);
        Debug.Log(RunServer(pos.x, pos.y, pos.z));
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
