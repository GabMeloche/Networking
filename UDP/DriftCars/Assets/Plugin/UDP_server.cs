using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
public class UDP_server : MonoBehaviour
{
    // Start is called before the first frame update
    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RunServer")]
    private static extern IntPtr RunServer();

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "NumberOfConnections")]
    private static extern int NumberOfConnections(IntPtr p_server);

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "DestroyServer")]
    private static extern void DestroyServer(IntPtr p_server);

    private IntPtr m_server;
    void Start()
    {
        m_server = RunServer();
    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log(NumberOfConnections(m_server));
    }

    void OnDestroy()
    {
        DestroyServer(m_server);
    }
}
